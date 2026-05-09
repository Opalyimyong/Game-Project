#include "game_data.h"

using namespace GameData;

const PowerPlantStats& GetPowerPlantStats(PlantType type, int level) {
    switch (type) {
        case PlantType::CoalPlant:
            switch (level) {
                case 1:
                    return { .type = PlantType::CoalPlant,
                             .level = level,
                             .build_cost = 50.0,
                             .maintenance_cost = 5.0,
                             .eff_mult = 0.8,
                             .min_input = 5.0,
                             .max_input = 10.0,
                             .waste_mult = 0.6 };
                case 2:
                    return { .type = PlantType::CoalPlant,
                             .level = level,
                             .build_cost = 230.0,
                             .maintenance_cost = 6.5,
                             .eff_mult = 1.12,
                             .min_input = 5.0,
                             .max_input = 10.0,
                             .waste_mult = 0.51 };
                case 3:
                    return { .type = PlantType::CoalPlant,
                             .level = level,
                             .build_cost = 600.0,
                             .maintenance_cost = 8.5,
                             .eff_mult = 1.44,
                             .min_input = 5.0,
                             .max_input = 10.0,
                             .waste_mult = 0.42 };
                default: return {};
            }

        case PlantType::GasPlant:
            switch (level) {
                case 1:
                    return { .type = PlantType::GasPlant,
                             .level = level,
                             .build_cost = 65.0,
                             .maintenance_cost = 4.0,
                             .eff_mult = 1.15,
                             .min_input = 4.0,
                             .max_input = 8.0,
                             .waste_mult = 0.5 };
                case 2:
                    return { .type = PlantType::GasPlant,
                             .level = level,
                             .build_cost = 320.0,
                             .maintenance_cost = 5.2,
                             .eff_mult = 1.61,
                             .min_input = 4.0,
                             .max_input = 8.0,
                             .waste_mult = 0.425 };
                case 3:
                    return { .type = PlantType::GasPlant,
                             .level = level,
                             .build_cost = 900.0,
                             .maintenance_cost = 6.8,
                             .eff_mult = 2.07,
                             .min_input = 4.0,
                             .max_input = 8.0,
                             .waste_mult = 0.35 };
                default: return {};
            }

        case PlantType::BiomassPlant:
            switch (level) {
                case 1:
                    return { .type = PlantType::BiomassPlant,
                             .level = level,
                             .build_cost = 90.0,
                             .maintenance_cost = 6.0,
                             .eff_mult = 0.75,
                             .min_input = 3.0,
                             .max_input = 6.0,
                             .waste_mult = 0.4 };
                case 2:
                    return { .type = PlantType::BiomassPlant,
                             .level = level,
                             .build_cost = 450.0,
                             .maintenance_cost = 7.8,
                             .eff_mult = 1.05,
                             .min_input = 3.0,
                             .max_input = 6.0,
                             .waste_mult = 0.34 };
                case 3:
                    return { .type = PlantType::BiomassPlant,
                             .level = level,
                             .build_cost = 1250.0,
                             .maintenance_cost = 10.2,
                             .eff_mult = 1.35,
                             .min_input = 3.0,
                             .max_input = 6.0,
                             .waste_mult = 0.28 };
                default: return {};
            }

        case PlantType::SolarPlant:
            switch (level) {
                case 1:
                    return { .type = PlantType::SolarPlant,
                             .level = level,
                             .build_cost = 120.0,
                             .maintenance_cost = 1.5,
                             .eff_mult = 1.0,
                             .min_input = 2.0,
                             .max_input = 8.0,
                             .waste_mult = 0.0 };
                case 2:
                    return { .type = PlantType::SolarPlant,
                             .level = level,
                             .build_cost = 720.0,
                             .maintenance_cost = 1.95,
                             .eff_mult = 1.4,
                             .min_input = 2.0,
                             .max_input = 8.0,
                             .waste_mult = 0.0 };
                case 3:
                    return { .type = PlantType::SolarPlant,
                             .level = level,
                             .build_cost = 1900.0,
                             .maintenance_cost = 2.55,
                             .eff_mult = 1.8,
                             .min_input = 2.0,
                             .max_input = 8.0,
                             .waste_mult = 0.0 };
                default: return {};
            }

        case PlantType::WindPlant:
            switch (level) {
                case 1:
                    return { .type = PlantType::WindPlant,
                             .level = level,
                             .build_cost = 150.0,
                             .maintenance_cost = 2.5,
                             .eff_mult = 1.05,
                             .min_input = 3.0,
                             .max_input = 9.0,
                             .waste_mult = 0.0 };
                case 2:
                    return { .type = PlantType::WindPlant,
                             .level = level,
                             .build_cost = 900.0,
                             .maintenance_cost = 3.25,
                             .eff_mult = 1.47,
                             .min_input = 3.0,
                             .max_input = 9.0,
                             .waste_mult = 0.0 };
                case 3:
                    return { .type = PlantType::WindPlant,
                             .level = level,
                             .build_cost = 2400.0,
                             .maintenance_cost = 4.25,
                             .eff_mult = 1.89,
                             .min_input = 3.0,
                             .max_input = 9.0,
                             .waste_mult = 0.0 };
                default: return {};
            }

        case PlantType::HydroPlant:
            switch (level) {
                case 1:
                    return { .type = PlantType::HydroPlant,
                             .level = level,
                             .build_cost = 180.0,
                             .maintenance_cost = 3.0,
                             .eff_mult = 0.35,
                             .min_input = 8.0,
                             .max_input = 15.0,
                             .waste_mult = 0.1 };
                case 2:
                    return { .type = PlantType::HydroPlant,
                             .level = level,
                             .build_cost = 1000.0,
                             .maintenance_cost = 3.9,
                             .eff_mult = 0.49,
                             .min_input = 8.0,
                             .max_input = 15.0,
                             .waste_mult = 0.085 };
                case 3:
                    return { .type = PlantType::HydroPlant,
                             .level = level,
                             .build_cost = 2700.0,
                             .maintenance_cost = 5.1,
                             .eff_mult = 0.63,
                             .min_input = 8.0,
                             .max_input = 15.0,
                             .waste_mult = 0.07 };
                default: return {};
            }

        case PlantType::NuclearPlant:
            switch (level) {
                case 1:
                    return { .type = PlantType::NuclearPlant,
                             .level = level,
                             .build_cost = 1200.0,
                             .maintenance_cost = 18.0,
                             .eff_mult = 2.5,
                             .min_input = 1.0,
                             .max_input = 2.0,
                             .waste_mult = 4.0 };
                case 2:
                    return { .type = PlantType::NuclearPlant,
                             .level = level,
                             .build_cost = 8000.0,
                             .maintenance_cost = 23.4,
                             .eff_mult = 3.5,
                             .min_input = 1.0,
                             .max_input = 2.0,
                             .waste_mult = 3.4 };
                case 3:
                    return { .type = PlantType::NuclearPlant,
                             .level = level,
                             .build_cost = 22000.0,
                             .maintenance_cost = 30.6,
                             .eff_mult = 4.5,
                             .min_input = 1.0,
                             .max_input = 2.0,
                             .waste_mult = 2.8 };
                default: return {};
            }

        default:
            return {};
    }
}

