// THE OPTIMIZED PAIRING ENGINE
#include "game_manage.h"
#include "node.h"
#include "building.h"
#include "iostream"


std::vector<Player *> players_ = {};
Player* getCurrentPlayer() {
    // Implementation for getting the current player
}
std::vector<std::vector<




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

//End Game Logic
bool isEndGame() {
    int active_players = 0;
    bool all_active_players_unpowered = true;

    for (const auto& player : players_) {
        if (player == nullptr || player->isBankrupt()) {
            continue;
        }

        ++active_players;
        // if (!player->isAllCityNodesUnpowered()) {
        //     all_active_players_unpowered = false;
        // }
    }

    if (active_players == 0) {
        return true; // All players are bankrupt, end the game
    }

    if (active_players == 2 && all_active_players_unpowered) {
        return true; // Two active players remain and all owned city nodes are unpowered
    }

    return false; // Game is still running
}

