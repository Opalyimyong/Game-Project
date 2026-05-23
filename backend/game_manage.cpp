// THE OPTIMIZED PAIRING ENGINE
#include "game_manage.h"
#include "node.h"
#include "building.h"
#include <iostream>
#include <algorithm>


std::vector<Player *> players_ = {};
Player* getCurrentPlayer() {
    // Implementation for getting the current player
}
std::vector<std::vector<std::unique_ptr<Node>>> nodeLayer; // Assuming this is defined and populated elsewhere
std::vector<std::vector<std::unique_ptr<Building>>> buildingLayer; // Assuming this

int main() {
    for (const auto& player : players_){    
        NextTurn();
    
        


    }


}




//map
void SyncBuildingsToNodes(
    std::vector<std::vector<std::unique_ptr<Node>>>& nodeLayer,
    std::vector<std::vector<std::unique_ptr<Building>>>& buildingLayer,
    int gridSize) 
{
    for (int r = 0; r < gridSize; r++) {
        for (int c = 0; c < gridSize; c++) {
            
            if (buildingLayer[r][c] != nullptr) {
                
                if (nodeLayer[r][c] != nullptr) {
                    
                    if (!nodeLayer[r][c]->HasBuilding()) {
                    
                        nodeLayer[r][c]->SetBuilding(std::move(buildingLayer[r][c]));
                        
                        std::cout << "Successfully paired new building to Node at [" << r << "][" << c << "]\n";
                    }
                }
            }
        }
    }
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
            return false; // At least one city is not powered yet
        }
    }
    return hasCityNode; // End game only when every city node is powered
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
    return winner; // Return the player with the highest asset value or nullptr if no players exist
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
    if (players_.empty()) {
        return nullptr;
    }
    return players_[turn_index_];
}

void NextTurn() {
    if (players_.empty()) {
        return;
    }
    if (isEndGame()) {
        return;
    }
    if (GetCurrentPlayer()->getActionPoints() <= 0) {
        std::cout << "Player " << GetCurrentPlayer()->getId() << " has no action points left. Moving to next player.\n";
        turn_index_ = (turn_index_ + 1) % players_.size();
    }
}

bool isPlayerLost(const Player* player) {
    if (player->isBankrupt() || player->getTotalWaste() >= 100.0) {
        std::cout << "Player " << player->getId() << " has lost the game.\n";
        return true;
    }
    return false;
}

