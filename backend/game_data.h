#pragma once
#include "game_const.h"

namespace GameData
{ // functiion for getter game's data
        PowerPlantStats GetPowerPlantStats(PlantType type, int level);
        ResourcePlantStats GetResourcePlantStats(ResourceType type, int level);
        CityData CreateCityData(CityType type, double min, double max, double charge);
        double GetTranportRate(TransportType item);
};