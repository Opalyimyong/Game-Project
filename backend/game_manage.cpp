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
    GetCurrentPlayer()->resetActionPoints();
    turn_index_ = (turn_index_ + 1) % players_.size();
    std::cout << "Turn advanced. Now Player " << GetCurrentPlayer()->getId() << "'s turn.\n";
    if (turn_index_ == 0) return true;
    return false;
}

void ProcessWorld() {
    std::cout << "--- END OF ROUND SIMULATION ---" << std::endl;
    
    // Step 0: Clear old city contracts
    for (auto* n : all_nodes_) {
        if (n->GetType() == NodeType::City) {
            if (auto* city = dynamic_cast<CityNode*>(n)) city->clearContracts();
        }
    }

    // Step 1: Mine Phase - all resource nodes process and generate output
    for (auto* n : all_nodes_) {
        if (n->GetType() == NodeType::Resource && n->HasBuilding()) {
            if (auto* rp = dynamic_cast<ResourcePlant*>(n->GetBuilding())) {
                rp->process();
                std::cout << "Resource node produced output" << std::endl;
            }
        }
    }
    // Push resource along links to power plants
    for (auto* link : all_links_) {
        if (link->GetNodeA()->GetType() == NodeType::Resource && link->GetNodeB()->GetType() == NodeType::Power) {
            auto* rp = dynamic_cast<ResourcePlant*>(link->GetNodeA()->GetBuilding());
            if (rp) link->GetNodeB()->recieveItem(link->GetOwner(), rp->getItem());
        }
    }

    // Step 2: Power Phase - power plants burn resources, generate energy and waste
    for (auto* n : all_nodes_) {
        if (n->GetType() == NodeType::Power && n->HasBuilding()) {
            if (auto* pp = dynamic_cast<PowerPlant*>(n->GetBuilding())) {
                if (pp->process()) {
                    double wasteGenerated = pp->getItem().waste_amount;
                    if (wasteGenerated > 0) {
                        // Find the link owner to assign waste to the correct player
                        for (auto* lk : all_links_) {
                            if (lk->GetNodeA() == n || lk->GetNodeB() == n) {
                                if (lk->GetOwner()) {
                                    lk->GetOwner()->addWaste(wasteGenerated);
                                    std::cout << "Added " << wasteGenerated << " waste to Player " << lk->GetOwner()->getId() << std::endl;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // Push energy along links to cities
    for (auto* lk : all_links_) {
        if (lk->GetNodeA()->GetType() == NodeType::Power && lk->GetNodeB()->GetType() == NodeType::City) {
            auto* pp = dynamic_cast<PowerPlant*>(lk->GetNodeA()->GetBuilding());
            if (pp) lk->GetNodeB()->recieveItem(lk->GetOwner(), pp->getItem());
        }
    }

    // Step 3: City Phase -> Payout Coins!
    for (auto* n : all_nodes_) {
        if (n->GetType() == NodeType::City) {
            auto* city = dynamic_cast<CityNode*>(n);
            if (city) {
                for (const auto& contract : city->getContracts()) {
                    if (contract.player) {
                        contract.player->addCoins(contract.amount_recieved);
                        std::cout << "Player " << contract.player->getId() << " earned " << contract.amount_recieved << " coins!" << std::endl;
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

