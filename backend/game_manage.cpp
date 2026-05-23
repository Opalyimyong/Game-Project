// THE OPTIMIZED PAIRING ENGINE
#include "node.h"
#include "building.h"
#include "iostream"
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