#pragma once
#include "game_const.h"

namespace GameData
{ // functiion for getter game's data
        PowerPlantStats GetPowerPlantStats(PlantType type);
        ResourcePlantStats GetResourcePlantStats(ResourceType type);
        CityData CreateCityData(CityType type);
        double GetTranportRate(TransportType item);
};