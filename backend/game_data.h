#pragma once
#include "game_const.h"

namespace GameData
{ // functiion for getter game's data
        PowerPlantStats GetPowerPlantStats(PlantType type, int level);
        ResourcePlantStats GetResourcePlantStats(ResourceType type, int level);
        CityData CreateCityData(CityType type, float min, float max, float charge);
        float GetTranportRate(TransportType item);
};