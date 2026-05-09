#pragma once
#include "game_const.h"

namespace GameData { //functiion for getter game's data
        const PowerPlantStats& GetPowerPlantStats (PlantType type, int level);
        const ResourcePlantStats& GetResourcePlantStats (ResourceType type, int level);
        CityData CreateCityData (CityType type, double min, double max, double charge);
        float GetTranportRate (TransportType item);
};