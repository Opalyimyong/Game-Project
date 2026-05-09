#pragma once
#include "game_type.h"

class GameData {
    public:
        const PowerPlantStats& GetPowerPlantStats (PlantType type, int level);
        const ResourcePlantStats& GetResourcePlantStats (ResourceType type);
};