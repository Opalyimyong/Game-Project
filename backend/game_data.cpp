#include "game_data.h"

using namespace GameData;

const PowerPlantStats& GetPowerPlantStats (PlantType type, int level) {
    switch (type)
    {
    case PlantType::CoalPlant :
        switch(level) {
            case 1 :
                return {.type = PlantType::CoalPlant, 
                        .level = level,
                        .build_cost = 50.0,
                        .maintenance_cost = 5.0,
                        .eff_mult = 0.8,
                        .min_input = 5.0,
                        .max_input = 10.0,
                        .waste_mult = 0.6 };
            case 2 :
                return {.type = PlantType::CoalPlant, 
                        .level = level,
                        .build_cost = 230.0, 
                        .maintenance_cost = 6.5,
                        .eff_mult = 1.12,
                        .min_input = 5.0,
                        .max_input = 10.0,
                        .waste_mult = 0.51 };
            case 3 :
                return {.type = PlantType::CoalPlant, 
                        .level = level,
                        .build_cost = 600.0, 
                        .maintenance_cost = 8.5,
                        .eff_mult = 1.44,
                        .min_input = 5.0,
                        .max_input = 10.0,
                        .waste_mult = 0.42 };
            default :
                return {};        
        }
    
    
    default:
        break;
    }
}