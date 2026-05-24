// THE OPTIMIZED PAIRING ENGINE
#include "game_manage.h"
#include "node.h"
#include "building.h"
#include <iostream>
#include <algorithm>


std::vector<Player *> players_ = {};
int turn_index_ = 0;
std::vector<Node *> all_nodes_ = {};
std::vector<Link *> all_links_ = {};
std::vector<std::vector<std::unique_ptr<Node>>> nodeLayer;
std::vector<std::vector<std::unique_ptr<Building>>> buildingLayer;

void ResetGame() {
    for (auto* p : players_) delete p;
    players_.clear();
    
    for (auto* n : all_nodes_) delete n;
    all_nodes_.clear();
    
    for (auto* l : all_links_) delete l;
    all_links_.clear();
    
    turn_index_ = 0;
    game_is_over_ = false;
    game_over_reason_ = "";
}

Node* GetOrCreateNode(int r, int c, std::string type, std::string subtype) {
    for (auto* n : all_nodes_) {
        if (n->GetX() == r && n->GetY() == c) return n;
    }
    
    Node* newNode = nullptr;
    if (type == "City") {
        CityType ct = (subtype == "Big") ? CityType::Big : CityType::Small;
        newNode = new CityNode(r, c, ct);
        // Default city data initialization
        dynamic_cast<CityNode*>(newNode)->setEnergyRange(); 
    } else if (type == "Resource") {
        newNode = new ResourceNode(r, c);
    } else if (type == "Power") {
        newNode = new PowerPlantNode(r, c);
    } else {
        newNode = new PowerPlantNode(r, c);
    }
    
    if (newNode) all_nodes_.push_back(newNode);
    return newNode;
}

//map
void SyncBuildingsToNodes(
    std::vector<std::vector<std::unique_ptr<Node>>>& nodeLayer,
    std::vector<std::vector<std::unique_ptr<Building>>>& buildingLayer,
    int gridSize) 
{
    // Keeping for backwards compatibility
}


std::chrono::steady_clock::time_point game_start_time_;
int game_timer_minutes_ = 10;
bool game_is_over_ = false;
std::string game_over_reason_ = "";

/*--------End Game Logic--------*/
bool IsAllCityNodesPowered() {
    bool hasCityNode = false;
    for (const auto& node : all_nodes_) {
        if (node->GetType() != NodeType::City) {
            continue;
        }
        hasCityNode = true;
        CityNode* cityNode = dynamic_cast<CityNode*>(node);
        if (!cityNode->isPowered()) {
            return false;
        }
    }
    return hasCityNode;
}

Player* GetWinner() {
    double highestAssetValue = -1.0;
    Player* winner = nullptr;
    for (const auto* player : players_) {
        if (player->calculateAssetValue() > highestAssetValue) {
            highestAssetValue = player->calculateAssetValue();
            winner = const_cast<Player*>(player);
        }
    }
    return winner;
}

bool isEndGame() {
    if (game_is_over_) return true;
    
    if (players_.empty()) return false;

    if (IsAllCityNodesPowered()) {
        std::cout << "All city nodes are powered. The game has ended.\n";
        game_over_reason_ = "All Cities Successfully Powered";
        game_is_over_ = true;
        return true;
    }
    
    int active_players = 0;
    for (const auto* p : players_) {
        if (!p->isGameOver()) active_players++;
    }
    if (active_players <= 1 && players_.size() > 1) {
        std::cout << "Other players are bankrupt. The game has ended.\n";
        game_over_reason_ = "Corporate Monopoly Established";
        game_is_over_ = true;
        return true;
    }
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(now - game_start_time_).count();
    if (elapsed >= game_timer_minutes_) {
        std::cout << "Timer expired! The game has ended.\n";
        game_over_reason_ = "Mission Timer Expired";
        game_is_over_ = true;
        return true;
    }
    
    return false;
}

/*--------Game Logic--------*/
Player* GetCurrentPlayer() {
    if (players_.empty()) return nullptr;
    return players_[turn_index_];
}

bool NextTurn() {
    if (players_.empty() || isEndGame()) return false;
    turn_index_ = (turn_index_ + 1) % players_.size();
    GetCurrentPlayer()->resetActionPoints();
    std::cout << "Turn advanced. Now Player " << GetCurrentPlayer()->getId() << "'s turn.\n";
    if (turn_index_ == 0) return true;
    return false;
}

void ProcessWorld() {
    std::cout << "--- END OF ROUND SIMULATION ---" << std::endl;
    std::cout << "Total nodes: " << all_nodes_.size() << ", Total links: " << all_links_.size() << std::endl;
    
    // Step 0: Clear old city contracts
    for (auto* n : all_nodes_) {
        if (n->GetType() == NodeType::City) {
            if (auto* city = dynamic_cast<CityNode*>(n)) city->clearContracts();
        }
    }

    // Step 1: Mine Phase - resource plants produce and waste is attributed to their link owner
    for (auto* n : all_nodes_) {
        if (n->GetType() == NodeType::Resource && n->HasBuilding()) {
            auto* rp = dynamic_cast<ResourcePlant*>(n->GetBuilding());
            if (rp && rp->process()) {
                std::cout << "[MINE] Resource node at (" << n->GetX() << "," << n->GetY() << ") produced " << rp->getItem().amount << " units, waste=" << rp->getItem().waste_amount << std::endl;
                // Attribute resource plant's own waste to whoever owns the link from it
                double rpWaste = rp->getItem().waste_amount;
                if (rpWaste > 0) {
                    for (auto* lk : all_links_) {
                        if (lk->GetNodeA() == n && lk->GetOwner()) {
                            lk->GetOwner()->addWaste(rpWaste);
                            std::cout << "[WASTE] Resource waste " << rpWaste << " -> Player " << lk->GetOwner()->getId() << std::endl;
                            break;
                        }
                    }
                }
            } else {
                std::cout << "[MINE] Resource node at (" << n->GetX() << "," << n->GetY() << ") has NO building or process failed" << std::endl;
            }
        }
    }
    // Push resource along links Resource->Power
    for (auto* lk : all_links_) {
        if (lk->GetNodeA()->GetType() == NodeType::Resource && lk->GetNodeB()->GetType() == NodeType::Power) {
            auto* rp = dynamic_cast<ResourcePlant*>(lk->GetNodeA()->GetBuilding());
            if (rp && rp->getItem().amount > 0) {
                double ship_cost = lk->GetDistance() * GameData::GetTranportRate(TransportType::Resource);
                if (lk->GetOwner() && !lk->GetOwner()->payAutoRunCost(ship_cost)) {
                    std::cout << "[LINK] Player " << lk->GetOwner()->getId() << " cannot afford shipping cost of " << ship_cost << " for Resource -> Power!" << std::endl;
                    continue;
                }
                std::cout << "[LINK] Pushing " << rp->getItem().amount << " resource from (" << lk->GetNodeA()->GetX() << "," << lk->GetNodeA()->GetY() << ") to power plant (Cost: " << ship_cost << ")" << std::endl;
                lk->GetNodeB()->recieveItem(lk->GetOwner(), rp->getItem());
            }
        }
    }

    // Step 2: Power Phase - power plants burn resources, generate energy and waste
    for (auto* n : all_nodes_) {
        if (n->GetType() == NodeType::Power && n->HasBuilding()) {
            auto* pp = dynamic_cast<PowerPlant*>(n->GetBuilding());
            if (pp) {
                std::cout << "[POWER] Power plant at (" << n->GetX() << "," << n->GetY() << ") resource_input=" << pp->getResourceInputAmount() << std::endl;
                if (pp->process()) {
                    double wasteGenerated = pp->getItem().waste_amount;
                    std::cout << "[POWER] Processed! energy=" << pp->getItem().amount << " waste=" << wasteGenerated << std::endl;
                    if (wasteGenerated > 0) {
                        for (auto* lk : all_links_) {
                            if ((lk->GetNodeA() == n || lk->GetNodeB() == n) && lk->GetOwner()) {
                                lk->GetOwner()->addWaste(wasteGenerated);
                                std::cout << "[WASTE] Power waste " << wasteGenerated << " -> Player " << lk->GetOwner()->getId() << std::endl;
                                break;
                            }
                        }
                    }
                } else {
                    std::cout << "[POWER] process() returned false (not enough fuel)" << std::endl;
                }
            }
        }
    }
    // Push energy along links Power->City
    for (auto* lk : all_links_) {
        if (lk->GetNodeA()->GetType() == NodeType::Power && lk->GetNodeB()->GetType() == NodeType::City) {
            auto* pp = dynamic_cast<PowerPlant*>(lk->GetNodeA()->GetBuilding());
            if (pp && pp->getItem().amount > 0) {
                double ship_cost = lk->GetDistance() * GameData::GetTranportRate(TransportType::Energy);
                if (lk->GetOwner() && !lk->GetOwner()->payAutoRunCost(ship_cost)) {
                    std::cout << "[LINK] Player " << lk->GetOwner()->getId() << " cannot afford shipping cost of " << ship_cost << " for Power -> City!" << std::endl;
                    continue;
                }
                std::cout << "[LINK] Pushing " << pp->getItem().amount << " energy to city (Cost: " << ship_cost << ")" << std::endl;
                lk->GetNodeB()->recieveItem(lk->GetOwner(), pp->getItem());
            }
        }
    }

    // Step 3: City Phase - payout coins!
    for (auto* n : all_nodes_) {
        if (n->GetType() == NodeType::City) {
            auto* city = dynamic_cast<CityNode*>(n);
            if (city) {
                for (const auto& contract : city->getContracts()) {
                    if (contract.player) {
                        contract.player->addCoins(contract.amount_recieved);
                        std::cout << "[CITY] Player " << contract.player->getId() << " earned " << contract.amount_recieved << " coins!" << std::endl;
                    }
                }
            }
        }
    }

}

bool isPlayerLost(const Player* player) {
    if (player->isGameOver()) {
        std::cout << "Player " << player->getId() << " has lost the game.\n";
        return true;
    }
    return false;
}

