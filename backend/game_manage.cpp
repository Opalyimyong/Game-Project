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
    if (IsAllCityNodesPowered()) {
        std::cout << "All city nodes are powered. The game has ended.\n";
        return true;
    }
    if (players_.size() <= 1) {
        std::cout << "Others players are bankrupt. The game has ended.\n"
                    << "The winner is " << (GetWinner() ? GetWinner()->getId() : "No one") << "\n";
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
            if (rp) {
                std::cout << "[LINK] Pushing " << rp->getItem().amount << " resource from (" << lk->GetNodeA()->GetX() << "," << lk->GetNodeA()->GetY() << ") to power plant" << std::endl;
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
            if (pp) {
                std::cout << "[LINK] Pushing " << pp->getItem().amount << " energy to city" << std::endl;
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
    if (player->isBankrupt() || player->getTotalWaste() >= 100.0) {
        std::cout << "Player " << player->getId() << " has lost the game.\n";
        return true;
    }
    return false;
}

