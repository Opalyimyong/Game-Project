#include "mongoose.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace {
constexpr int MAP_WIDTH = 50;
constexpr int MAP_HEIGHT = 50;
constexpr int STARTING_COINS = 500;
constexpr int STARTING_AP = 3;
constexpr double LINK_COST_PER_TILE = 1.0;

struct PlayerState {
  int id = 1;
  double coins = STARTING_COINS;
  int ap = STARTING_AP;
  double waste = 0.0;
};

struct NodeState {
  int id = 0;
  int x = 0;
  int y = 0;
  std::string type = "None";        // City, Power, Resource
  std::string subtype = "";         // Small/Big or environment/resource hint
  bool hasBuilding = false;
  std::string buildingType = "";    // ResourcePlant or PowerPlant
  std::string plantType = "";       // CoalPlant, SolarPlant, ...
  int owner = 0;
  double energy = 0.0;
  double demandMin = 0.0;
  double demandMax = 0.0;
  double resourceOutput = 0.0;
  double energyOutput = 0.0;
};

struct LinkState { int a = 0; int b = 0; int owner = 0; };

struct GameState {
  bool started = false;
  int turn = 1;
  int currentPlayer = 1;
  std::vector<PlayerState> players;
  std::vector<NodeState> nodes;
  std::vector<LinkState> links;
  std::string message;
  std::string winner;
} game;

std::string esc(const std::string& s) {
  std::string out;
  for (char c : s) {
    if (c == '"' || c == '\\') out += '\\';
    out += c;
  }
  return out;
}

std::string jsonString(const std::string& s) { return "\"" + esc(s) + "\""; }

double dist(const NodeState& a, const NodeState& b) {
  double dx = static_cast<double>(a.x - b.x);
  double dy = static_cast<double>(a.y - b.y);
  return std::sqrt(dx * dx + dy * dy);
}

NodeState* findNode(int id) {
  for (auto& n : game.nodes) if (n.id == id) return &n;
  return nullptr;
}

PlayerState* currentPlayer() {
  for (auto& p : game.players) if (p.id == game.currentPlayer) return &p;
  return nullptr;
}

bool cellUsed(int x, int y) {
  for (const auto& n : game.nodes) if (n.x == x && n.y == y) return true;
  return false;
}

void addNode(int& nextId, int x, int y, const std::string& type, const std::string& subtype) {
  NodeState n;
  n.id = nextId++;
  n.x = x;
  n.y = y;
  n.type = type;
  n.subtype = subtype;
  if (type == "City") {
    if (subtype == "Big") { n.demandMin = 28; n.demandMax = 45; }
    else { n.demandMin = 14; n.demandMax = 24; }
  }
  game.nodes.push_back(n);
}

void addRandomNodes(std::mt19937& rng, int& nextId, int count, const std::string& type, const std::vector<std::string>& subtypes) {
  std::uniform_int_distribution<int> coord(2, 47);
  for (int i = 0; i < count; ++i) {
    int x, y, guard = 0;
    do { x = coord(rng); y = coord(rng); guard++; } while (cellUsed(x, y) && guard < 500);
    addNode(nextId, x, y, type, subtypes[i % subtypes.size()]);
  }
}

std::string stateJson() {
  std::ostringstream os;
  os << "{\"started\":" << (game.started ? "true" : "false")
     << ",\"width\":" << MAP_WIDTH << ",\"height\":" << MAP_HEIGHT
     << ",\"turn\":" << game.turn << ",\"currentPlayer\":" << game.currentPlayer
     << ",\"message\":" << jsonString(game.message)
     << ",\"winner\":" << jsonString(game.winner) << ",\"players\":[";
  for (size_t i = 0; i < game.players.size(); ++i) {
    const auto& p = game.players[i];
    if (i) os << ',';
    os << "{\"id\":" << p.id << ",\"coins\":" << p.coins << ",\"ap\":" << p.ap << ",\"waste\":" << p.waste << '}';
  }
  os << "],\"nodes\":[";
  for (size_t i = 0; i < game.nodes.size(); ++i) {
    const auto& n = game.nodes[i];
    if (i) os << ',';
    os << "{\"id\":" << n.id << ",\"x\":" << n.x << ",\"y\":" << n.y
       << ",\"type\":" << jsonString(n.type) << ",\"subtype\":" << jsonString(n.subtype)
       << ",\"hasBuilding\":" << (n.hasBuilding ? "true" : "false")
       << ",\"buildingType\":" << jsonString(n.buildingType) << ",\"plantType\":" << jsonString(n.plantType)
       << ",\"owner\":" << n.owner << ",\"energy\":" << n.energy
       << ",\"demandMin\":" << n.demandMin << ",\"demandMax\":" << n.demandMax
       << ",\"resourceOutput\":" << n.resourceOutput << ",\"energyOutput\":" << n.energyOutput << '}';
  }
  os << "],\"links\":[";
  for (size_t i = 0; i < game.links.size(); ++i) {
    const auto& l = game.links[i];
    if (i) os << ',';
    os << "{\"a\":" << l.a << ",\"b\":" << l.b << ",\"owner\":" << l.owner << '}';
  }
  os << "]}";
  return os.str();
}

std::string envelope(const std::string& type) {
  return "{\"type\":" + jsonString(type) + ",\"state\":" + stateJson() + "}";
}

int jsonInt(const std::string& body, const std::string& key, int fallback = 0) {
  std::string pat = "\"" + key + "\":";
  size_t p = body.find(pat);
  if (p == std::string::npos) return fallback;
  p += pat.size();
  return std::atoi(body.c_str() + p);
}

std::string jsonStr(const std::string& body, const std::string& key, const std::string& fallback = "") {
  std::string pat = "\"" + key + "\":";
  size_t p = body.find(pat);
  if (p == std::string::npos) return fallback;
  p += pat.size();
  while (p < body.size() && body[p] != '"') ++p;
  if (p >= body.size()) return fallback;
  size_t start = ++p;
  while (p < body.size() && body[p] != '"') ++p;
  return body.substr(start, p - start);
}

void startGame(int playerCount) {
  playerCount = std::max(1, std::min(3, playerCount));
  game = GameState{};
  game.started = true;
  game.turn = 1;
  game.currentPlayer = 1;
  for (int i = 1; i <= playerCount; ++i) game.players.push_back(PlayerState{i, STARTING_COINS, STARTING_AP, 0});

  std::mt19937 rng(42); // fixed seed so the map is stable/debuggable
  int nextId = 1;
  addRandomNodes(rng, nextId, 5, "City", {"Small"});
  addRandomNodes(rng, nextId, 3, "City", {"Big"});
  addRandomNodes(rng, nextId, 12, "Power", {"Solar", "Wind", "Water", "Plain"});
  addRandomNodes(rng, nextId, 18, "Resource", {"Coal", "Gas", "Biomass", "Uranium"});
  game.message = "Mission initialized. Build plants, link Resource → Power → City, then End Turn.";
}

double resourceBuildCost(const std::string& type) {
  if (type == "Uranium") return 100;
  if (type == "Gas") return 55;
  if (type == "Biomass") return 45;
  return 40;
}

double powerBuildCost(const std::string& type) {
  if (type == "CoalPlant") return 50;
  if (type == "GasPlant") return 65;
  if (type == "BiomassPlant") return 90;
  if (type == "SolarPlant") return 120;
  if (type == "WindPlant") return 150;
  if (type == "HydroPlant") return 200;
  if (type == "NuclearPlant") return 500;
  return 100;
}

bool pay(PlayerState& p, double cost, int ap = 1) {
  if (p.ap < ap) { game.message = "Not enough Action Points."; return false; }
  if (p.coins < cost) { game.message = "Not enough coins."; return false; }
  p.ap -= ap;
  p.coins -= cost;
  return true;
}

void buildResource(int nodeId, const std::string& resourceType) {
  PlayerState* p = currentPlayer(); NodeState* n = findNode(nodeId);
  if (!p || !n) { game.message = "Invalid player or node."; return; }
  if (n->type != "Resource") { game.message = "Resource Plant must be built on a Resource Node."; return; }
  if (n->hasBuilding) { game.message = "This node already has a building."; return; }
  double cost = resourceBuildCost(resourceType);
  if (!pay(*p, cost)) return;
  n->hasBuilding = true; n->buildingType = "ResourcePlant"; n->plantType = resourceType; n->owner = p->id;
  n->resourceOutput = (resourceType == "Uranium") ? 4 : (resourceType == "Gas" ? 7 : 8);
  p->waste += (resourceType == "Biomass") ? 1 : 2;
  game.message = "Resource Plant built.";
}

void buildPower(int nodeId, const std::string& plantType) {
  PlayerState* p = currentPlayer(); NodeState* n = findNode(nodeId);
  if (!p || !n) { game.message = "Invalid player or node."; return; }
  if (n->type != "Power") { game.message = "Power Plant must be built on a Power Node."; return; }
  if (n->hasBuilding) { game.message = "This node already has a building."; return; }
  double cost = powerBuildCost(plantType);
  if (!pay(*p, cost)) return;
  n->hasBuilding = true; n->buildingType = "PowerPlant"; n->plantType = plantType; n->owner = p->id;
  if (plantType == "SolarPlant") n->energyOutput = (n->subtype == "Solar" ? 11 : 7);
  if (plantType == "WindPlant") n->energyOutput = (n->subtype == "Wind" ? 12 : 8);
  if (plantType == "HydroPlant") n->energyOutput = (n->subtype == "Water" ? 15 : 4);
  game.message = "Power Plant built.";
}

bool linkExists(int a, int b) {
  for (const auto& l : game.links) if ((l.a == a && l.b == b) || (l.a == b && l.b == a)) return true;
  return false;
}

void addLink(int a, int b) {
  PlayerState* p = currentPlayer(); NodeState* na = findNode(a); NodeState* nb = findNode(b);
  if (!p || !na || !nb || a == b) { game.message = "Invalid link."; return; }
  bool valid = (na->type == "Resource" && nb->type == "Power") || (na->type == "Power" && nb->type == "Resource") ||
               (na->type == "Power" && nb->type == "City") || (na->type == "City" && nb->type == "Power");
  if (!valid) { game.message = "Only Resource ↔ Power and Power ↔ City links are allowed."; return; }
  if (linkExists(a, b)) { game.message = "Link already exists."; return; }
  double cost = std::ceil(dist(*na, *nb) * LINK_COST_PER_TILE);
  if (!pay(*p, cost)) return;
  game.links.push_back(LinkState{a, b, p->id});
  game.message = "Link created.";
}

void processTurn() {
  for (auto& n : game.nodes) if (n.type == "City") n.energy = 0;

  for (auto& power : game.nodes) {
    if (power.type != "Power" || !power.hasBuilding) continue;
    double input = 0;
    for (const auto& l : game.links) {
      NodeState* other = nullptr;
      if (l.a == power.id) other = findNode(l.b);
      if (l.b == power.id) other = findNode(l.a);
      if (other && other->type == "Resource" && other->hasBuilding && other->owner == power.owner) input += other->resourceOutput;
    }
    if (power.plantType == "CoalPlant") power.energyOutput = std::min(input, 10.0) * 0.8;
    else if (power.plantType == "GasPlant") power.energyOutput = std::min(input, 8.0) * 1.15;
    else if (power.plantType == "BiomassPlant") power.energyOutput = std::min(input, 6.0) * 0.75;
    else if (power.plantType == "NuclearPlant") power.energyOutput = std::min(input, 5.0) * 5.0;

    for (const auto& l : game.links) {
      NodeState* city = nullptr;
      if (l.a == power.id) city = findNode(l.b);
      if (l.b == power.id) city = findNode(l.a);
      if (city && city->type == "City") {
        city->energy += power.energyOutput;
        PlayerState* owner = nullptr;
        for (auto& p : game.players) if (p.id == power.owner) owner = &p;
        if (owner) {
          owner->coins += power.energyOutput * (city->subtype == "Big" ? 5.0 : 4.0);
          owner->waste += (power.plantType == "SolarPlant" || power.plantType == "WindPlant" || power.plantType == "HydroPlant") ? 0.0 : power.energyOutput * 0.18;
        }
      }
    }
  }

  bool allPowered = true;
  for (const auto& n : game.nodes) if (n.type == "City" && n.energy < n.demandMin) allPowered = false;
  if (allPowered) {
    auto best = std::max_element(game.players.begin(), game.players.end(), [](const PlayerState& a, const PlayerState& b) {
      return (a.coins - a.waste * 2.0) < (b.coins - b.waste * 2.0);
    });
    game.winner = best == game.players.end() ? "" : ("Player " + std::to_string(best->id));
    game.message = "All cities are powered. Winner: " + game.winner;
  } else {
    game.message = "Turn processed.";
  }
}

void endTurn() {
  if (!game.started) return;
  processTurn();
  if (!game.winner.empty()) return;
  PlayerState* p = currentPlayer();
  if (p) p->ap = STARTING_AP;
  game.currentPlayer++;
  if (game.currentPlayer > static_cast<int>(game.players.size())) { game.currentPlayer = 1; game.turn++; }
}

void sendText(mg_connection* c, const std::string& text) {
  mg_ws_send(c, text.c_str(), text.size(), WEBSOCKET_OP_TEXT);
}

void handleWs(mg_connection* c, const std::string& payload) {
  std::string type = jsonStr(payload, "type");
  if (type == "START_GAME") { startGame(jsonInt(payload, "players", 3)); sendText(c, envelope("GAME_STARTED")); }
  else if (type == "GET_STATE") { sendText(c, envelope("STATE")); }
  else if (type == "BUILD_RESOURCE") { buildResource(jsonInt(payload, "nodeId"), jsonStr(payload, "resourceType", "Coal")); sendText(c, envelope("STATE")); }
  else if (type == "BUILD_POWER") { buildPower(jsonInt(payload, "nodeId"), jsonStr(payload, "plantType", "CoalPlant")); sendText(c, envelope("STATE")); }
  else if (type == "ADD_LINK") { addLink(jsonInt(payload, "a"), jsonInt(payload, "b")); sendText(c, envelope("STATE")); }
  else if (type == "END_TURN") { endTurn(); sendText(c, envelope("STATE")); }
  else sendText(c, "{\"type\":\"ERROR\",\"message\":\"Unknown command\"}");
}

void serveFile(mg_connection* c, mg_http_message* hm) {
  std::string uri(hm->uri.buf, hm->uri.len);
  if (uri == "/") uri = "/index.html";
  std::string path = "." + uri;
  mg_http_serve_opts opts{};
  opts.root_dir = ".";
  mg_http_serve_file(c, hm, path.c_str(), &opts);
}

void fn(mg_connection* c, int ev, void* ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    auto* hm = static_cast<mg_http_message*>(ev_data);
    if (mg_match(hm->uri, mg_str("/ws"), nullptr)) mg_ws_upgrade(c, hm, nullptr);
    else serveFile(c, hm);
  } else if (ev == MG_EV_WS_OPEN) {
    std::cout << "Client connected\n";
  } else if (ev == MG_EV_WS_MSG) {
    auto* wm = static_cast<mg_ws_message*>(ev_data);
    handleWs(c, std::string(wm->data.buf, wm->data.len));
  }
}
} // namespace

int main() {
  mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0:18888", fn, nullptr);
  std::cout << "Energy Not Included server: http://localhost:18888\n";
  for (;;) mg_mgr_poll(&mgr, 100);
  mg_mgr_free(&mgr);
  return 0;
}
