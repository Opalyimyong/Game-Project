#include "mongoose.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include "game_manage.h"
#include "building.h"

void broadcastGameState(struct mg_mgr *mgr) {
    std::stringstream ss;
    ss << "{\"action\":\"game_state\",\"players\":[";
    for (size_t i = 0; i < players_.size(); i++) {
        Player* p = players_[i];
        ss << "{\"id\":\"" << p->getId() << "\",\"ap\":" << p->getActionPoints() 
           << ",\"coins\":" << p->getCoins() << ",\"waste\":" << p->getTotalWaste() << "}";
        if (i < players_.size() - 1) ss << ",";
    }
    ss << "],\"nodes\":[";
    bool firstNode = true;
    for (const auto* node : all_nodes_) {
        if (node->GetType() == NodeType::City) {
            auto* cityNode = dynamic_cast<const CityNode*>(node);
            if (cityNode) {
                if (!firstNode) ss << ",";
                ss << "{\"r\":" << cityNode->GetX() << ",\"c\":" << cityNode->GetY() 
                   << ",\"energy\":" << cityNode->EnergyNow() << ",\"isPowered\":" << (cityNode->isPowered() ? "true" : "false") << "}";
                firstNode = false;
            }
        }
    }
    ss << "]}";
    
    std::string response = ss.str();
    std::cout << "Broadcasting Game State: " << response << std::endl;
    
    for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
        if (c->is_websocket) {
            mg_ws_send(c, response.c_str(), response.length(), WEBSOCKET_OP_TEXT);
        }
    }
}

void broadcastGameOver(struct mg_mgr *mgr) {
    if (!game_is_over_) return;
    
    Player* winner = GetWinner();
    std::string winner_name = winner ? winner->getId() : "None";
    double winner_score = winner ? winner->calculateAssetValue() : 0.0;
    
    std::stringstream ss;
    ss << "{\"action\":\"game_over\",\"reason\":\"" << game_over_reason_ 
       << "\",\"winner_name\":\"" << winner_name << "\",\"winner_score\":" << winner_score << "}";
       
    std::string response = ss.str();
    std::cout << "Broadcasting Game Over: " << response << std::endl;
    
    for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
        if (c->is_websocket) {
            mg_ws_send(c, response.c_str(), response.length(), WEBSOCKET_OP_TEXT);
        }
    }
}

bool has_broadcasted_game_over = false;

void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_POLL) {
        if (!has_broadcasted_game_over && !players_.empty() && isEndGame()) {
            has_broadcasted_game_over = true;
            broadcastGameOver(c->mgr);
        }
    } else if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        mg_ws_upgrade(c, hm, NULL); 
    } else if (ev == MG_EV_WS_OPEN) {
        std::cout << "Client connected successfully!" << std::endl;
        if (players_.empty()) {
            players_.push_back(new Player("p1"));
            players_.push_back(new Player("p2"));
        }
        broadcastGameState(c->mgr);

    } else if (ev == MG_EV_WS_MSG) {
        struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
        std::string payload(wm->data.buf, wm->data.len);
        std::cout << "Received: " << payload << std::endl;

        if (payload.find("START_GAME") != std::string::npos) {
            ResetGame();
            if (payload.find("\"id\":\"p1\"") != std::string::npos) players_.push_back(new Player("p1"));
            if (payload.find("\"id\":\"p2\"") != std::string::npos) players_.push_back(new Player("p2"));
            if (payload.find("\"id\":\"p3\"") != std::string::npos) players_.push_back(new Player("p3"));
            if (payload.find("\"id\":\"p4\"") != std::string::npos) players_.push_back(new Player("p4"));
            
            size_t tm_pos = payload.find("\"timer_minutes\":");
            if (tm_pos != std::string::npos) {
                game_timer_minutes_ = std::stoi(payload.substr(tm_pos + 16));
            }
            
            size_t tc_pos = payload.find("\"total_cities\":");
            if (tc_pos != std::string::npos) {
                total_cities_in_game_ = std::stoi(payload.substr(tc_pos + 15));
            } else {
                total_cities_in_game_ = 8; // fallback to hardcoded map data
            }
            game_start_time_ = std::chrono::steady_clock::now();
            game_is_over_ = false;
            has_broadcasted_game_over = false;
            game_over_reason_ = "";
            
            std::string response = "{\"status\":\"started\"}";
            mg_ws_send(c, response.c_str(), response.length(), WEBSOCKET_OP_TEXT);
            broadcastGameState(c->mgr);
        } else if (payload.find("\"action\":\"build\"") != std::string::npos) {
            std::string playerId = "p1";
            if (payload.find("\"player\":\"p2\"") != std::string::npos) playerId = "p2";
            else if (payload.find("\"player\":\"p3\"") != std::string::npos) playerId = "p3";
            else if (payload.find("\"player\":\"p4\"") != std::string::npos) playerId = "p4";
            
            Player* p = nullptr;
            for (auto* pl : players_) { if (pl->getId() == playerId) { p = pl; break; } }
            if (!p && !players_.empty()) p = players_[0];
            
            double cost = 0.0;
            Building* newBuilding = nullptr;

            std::string typeStr = "Power";
            std::string subtypeStr = "PowerPlant";
            if (payload.find("\"type\":\"ResourcePlant\"") != std::string::npos) {
                typeStr = "Resource";
                ResourceType rt = ResourceType::Coal;
                if (payload.find("\"subtype\":\"Gas\"") != std::string::npos) { rt = ResourceType::Gas; subtypeStr = "Gas"; }
                else if (payload.find("\"subtype\":\"Biomass\"") != std::string::npos) { rt = ResourceType::Biomass; subtypeStr = "Biomass"; }
                else if (payload.find("\"subtype\":\"Uranium\"") != std::string::npos) { rt = ResourceType::Uranium; subtypeStr = "Uranium"; }
                
                cost = GameData::GetResourcePlantStats(rt).build_cost;
                newBuilding = new ResourcePlant(rand(), p, {0,0}, rt, 1);
            } else if (payload.find("\"type\":\"PowerPlant\"") != std::string::npos) {
                typeStr = "Power";
                PlantType pt = PlantType::CoalPlant;
                if (payload.find("\"subtype\":\"Gas Plant\"") != std::string::npos) { pt = PlantType::GasPlant; subtypeStr = "Gas Plant"; }
                else if (payload.find("\"subtype\":\"Biomass Plant\"") != std::string::npos) { pt = PlantType::BiomassPlant; subtypeStr = "Biomass Plant"; }
                else if (payload.find("\"subtype\":\"Solar Plant\"") != std::string::npos) { pt = PlantType::SolarPlant; subtypeStr = "Solar Plant"; }
                else if (payload.find("\"subtype\":\"Wind Plant\"") != std::string::npos) { pt = PlantType::WindPlant; subtypeStr = "Wind Plant"; }
                else if (payload.find("\"subtype\":\"Hydro Plant\"") != std::string::npos) { pt = PlantType::HydroPlant; subtypeStr = "Hydro Plant"; }
                else if (payload.find("\"subtype\":\"Nuclear Plant\"") != std::string::npos) { pt = PlantType::NuclearPlant; subtypeStr = "Nuclear Plant"; }
                
                cost = GameData::GetPowerPlantStats(pt).build_cost;
                PowerPlant* pp = new PowerPlant(rand(), p, {0,0}, pt);
                // DO NOT mock input anymore, since we have real Resource nodes!
                newBuilding = pp;
            }

            int r = 0, col = 0;
            size_t r_pos = payload.find("\"r\":");
            if (r_pos != std::string::npos) r = std::stoi(payload.substr(r_pos + 4));
            size_t c_pos = payload.find("\"c\":");
            if (c_pos != std::string::npos) col = std::stoi(payload.substr(c_pos + 4));

            if (p && p->executeManualAction(cost)) {
                if (newBuilding) {
                    Node* n = GetOrCreateNode(r, col, typeStr, subtypeStr);
                    if (n->GetType() == NodeType::Power) {
                        auto* ppn = dynamic_cast<PowerPlantNode*>(n);
                        if (ppn) {
                            double sl = 0.0, wnd = 0.0; bool wtr = false;
                            size_t p_sl = payload.find("\"sunlight\":");
                            if (p_sl != std::string::npos) sl = std::stod(payload.substr(p_sl + 11));
                            size_t p_wn = payload.find("\"wind\":");
                            if (p_wn != std::string::npos) wnd = std::stod(payload.substr(p_wn + 7));
                            size_t p_wt = payload.find("\"water\":");
                            if (p_wt != std::string::npos) {
                                std::string val = payload.substr(p_wt + 8, 4);
                                wtr = (val == "true");
                            }
                            ppn->setEnvironmentStats(sl, wnd, wtr);
                        }
                    }
                    n->SetBuilding(std::unique_ptr<Building>(newBuilding));
                }
                std::cout << "Player built structure successfully." << std::endl;
            } else {
                std::cout << "Build rejected! Insufficient AP or Coins." << std::endl;
                if (newBuilding) delete newBuilding;
            }
            broadcastGameState(c->mgr);

        } else if (payload.find("\"action\":\"link\"") != std::string::npos) {
            std::string playerId = "p1";
            if (payload.find("\"player\":\"p2\"") != std::string::npos) playerId = "p2";
            else if (payload.find("\"player\":\"p3\"") != std::string::npos) playerId = "p3";
            else if (payload.find("\"player\":\"p4\"") != std::string::npos) playerId = "p4";
            
            Player* p = nullptr;
            for (auto* pl : players_) { if (pl->getId() == playerId) { p = pl; break; } }
            if (!p && !players_.empty()) p = players_[0];

            TransportType tType = TransportType::Resource;
            if (payload.find("\"transport_type\":\"Energy\"") != std::string::npos) {
                tType = TransportType::Energy;
            }

            int from_r = 0, from_c = 0, to_r = 0, to_c = 0;
            std::string from_type = "Resource", from_subtype = "";
            std::string to_type = "Power", to_subtype = "";

            // Parse "from" object boundaries first, then search within them
            size_t fr_pos = payload.find("\"from\":");
            size_t to_start = payload.find("\"to\":");
            if (fr_pos != std::string::npos && to_start != std::string::npos) {
                // parse from block (between fr_pos and to_start)
                std::string from_block = payload.substr(fr_pos, to_start - fr_pos);
                size_t r_p = from_block.find("\"r\":");
                if (r_p != std::string::npos) from_r = std::stoi(from_block.substr(r_p + 4));
                size_t c_p = from_block.find("\"c\":");
                if (c_p != std::string::npos) from_c = std::stoi(from_block.substr(c_p + 4));
                size_t t_p = from_block.find("\"type\":\"");
                if (t_p != std::string::npos) {
                    size_t t_end = from_block.find("\"", t_p + 8);
                    from_type = from_block.substr(t_p + 8, t_end - (t_p + 8));
                }
                size_t st_p = from_block.find("\"subtype\":\"");
                if (st_p != std::string::npos) {
                    size_t st_end = from_block.find("\"", st_p + 11);
                    from_subtype = from_block.substr(st_p + 11, st_end - (st_p + 11));
                }

                // parse to block (from to_start to end)
                size_t player_pos = payload.find(",\"player\":", to_start);
                std::string to_block = (player_pos != std::string::npos)
                    ? payload.substr(to_start, player_pos - to_start)
                    : payload.substr(to_start);
                size_t r_p2 = to_block.find("\"r\":");
                if (r_p2 != std::string::npos) to_r = std::stoi(to_block.substr(r_p2 + 4));
                size_t c_p2 = to_block.find("\"c\":");
                if (c_p2 != std::string::npos) to_c = std::stoi(to_block.substr(c_p2 + 4));
                size_t t_p2 = to_block.find("\"type\":\"");
                if (t_p2 != std::string::npos) {
                    size_t t_end2 = to_block.find("\"", t_p2 + 8);
                    to_type = to_block.substr(t_p2 + 8, t_end2 - (t_p2 + 8));
                }
                size_t st_p2 = to_block.find("\"subtype\":\"");
                if (st_p2 != std::string::npos) {
                    size_t st_end2 = to_block.find("\"", st_p2 + 11);
                    to_subtype = to_block.substr(st_p2 + 11, st_end2 - (st_p2 + 11));
                }
            }

            double distance = std::sqrt(std::pow(from_r - to_r, 2) + std::pow(from_c - to_c, 2));
            double cost = distance * GameData::GetTranportRate(tType);

            bool valid_link = true;
            if (from_type == "Resource" && to_type == "Power") {
                if (from_subtype == "Coal" && to_subtype != "Coal Plant") valid_link = false;
                else if (from_subtype == "Gas" && to_subtype != "Gas Plant") valid_link = false;
                else if (from_subtype == "Biomass" && to_subtype != "Biomass Plant") valid_link = false;
                else if (from_subtype == "Uranium" && to_subtype != "Nuclear Plant") valid_link = false;
            }

            // Check outgoing link restriction
            for (auto* lk : all_links_) {
                if (lk->GetNodeA() && lk->GetNodeA()->GetX() == from_r && lk->GetNodeA()->GetY() == from_c) {
                    valid_link = false;
                    std::cout << "Link rejected! Node already has an outgoing link." << std::endl;
                    break;
                }
            }

            // Check incoming link requirement for non-passive power plants connecting to a city
            if (valid_link && from_type == "Power" && to_type == "City") {
                bool is_passive = (from_subtype == "Solar Plant" || from_subtype == "Wind Plant" || from_subtype == "Hydro Plant");
                if (!is_passive) {
                    bool has_incoming = false;
                    for (auto* lk : all_links_) {
                        if (lk->GetNodeB() && lk->GetNodeB()->GetX() == from_r && lk->GetNodeB()->GetY() == from_c) {
                            has_incoming = true;
                            break;
                        }
                    }
                    if (!has_incoming) {
                        valid_link = false;
                        std::cout << "Link rejected! Power plant requires incoming resource link." << std::endl;
                    }
                }
            }

            if (!valid_link) {
                std::cout << "Link rejected! Validation failed." << std::endl;
            } else if (p && p->executeManualAction(cost)) {
                Node* nodeA = GetOrCreateNode(from_r, from_c, from_type, from_subtype);
                Node* nodeB = GetOrCreateNode(to_r, to_c, to_type, to_subtype);
                
                bool already_linked = false;
                for (auto* lk : all_links_) {
                    if (lk->GetOwner() == p && lk->GetNodeB() == nodeB) {
                        already_linked = true;
                        break;
                    }
                }
                
                Link* newLink = new Link(nodeA, nodeB, p);
                all_links_.push_back(newLink);
                std::cout << "Player created a link from (" << from_r << "," << from_c << ") to (" << to_r << "," << to_c << ") with cost " << cost << std::endl;
                
                if (!already_linked && nodeB->GetType() == NodeType::City) {
                    p->accessCityNode(dynamic_cast<CityNode*>(nodeB));
                }
            } else {
                std::cout << "Link rejected! Insufficient AP or Coins." << std::endl;
            }
            broadcastGameState(c->mgr);

        } else if (payload.find("\"action\":\"dispose_waste\"") != std::string::npos) {
            std::string playerId = "p1";
            if (payload.find("\"player\":\"p2\"") != std::string::npos) playerId = "p2";
            else if (payload.find("\"player\":\"p3\"") != std::string::npos) playerId = "p3";
            else if (payload.find("\"player\":\"p4\"") != std::string::npos) playerId = "p4";
            
            Player* p = nullptr;
            for (auto* pl : players_) { if (pl->getId() == playerId) { p = pl; break; } }
            if (!p && !players_.empty()) p = players_[0];

            size_t pos = payload.find("\"amount\":");
            if (pos != std::string::npos && p) {
                double amount = std::stod(payload.substr(pos + 9));
                if (p->disposeWaste(amount)) {
                    std::cout << "Player disposed " << amount << " waste." << std::endl;
                } else {
                    std::cout << "Dispose rejected! Insufficient AP or Coins." << std::endl;
                }
            }
            broadcastGameState(c->mgr);

        } else if (payload.find("\"action\":\"end_turn\"") != std::string::npos) {
            std::cout << "Player ended their turn." << std::endl;
            if (NextTurn()) {
                std::cout << "--- GLOBAL ROUND ENDED ---" << std::endl;
                ProcessWorld();
            }
            broadcastGameState(c->mgr);
        }
    }
}

int main() {
struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    
    // Using port 18888 as decided to avoid conflicts
    mg_http_listen(&mgr, "http://0.0.0.0:18888", fn, NULL); 
    
    std::cout << "Server started on http://localhost:18888" << std::endl;

    for (;;) mg_mgr_poll(&mgr, 100); // Faster polling for better game response
    
    mg_mgr_free(&mgr);
    return 0;
}