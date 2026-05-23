#include "game_data.h"

namespace GameData
{

    // Data for PowerPlant Node & Building in each type+level
    PowerPlantStats GetPowerPlantStats(PlantType type, int level)
    {
        switch (type)
        {
        case PlantType::CoalPlant:
            switch (level)
            {
            case 1:
                return PowerPlantStats{.type = PlantType::CoalPlant,
                                       .level = level,
                                       .build_cost = 50.0,
                                       .maintenance_cost = 5.0,
                                       .eff_mult = 0.8,
                                       .min_input = 5.0,
                                       .max_input = 10.0,
                                       .waste_mult = 0.6};
            case 2:
                return PowerPlantStats{.type = PlantType::CoalPlant,
                                       .level = level,
                                       .build_cost = 230.0,
                                       .maintenance_cost = 6.5,
                                       .eff_mult = 1.12,
                                       .min_input = 5.0,
                                       .max_input = 10.0,
                                       .waste_mult = 0.51};
            case 3:
                return PowerPlantStats{.type = PlantType::CoalPlant,
                                       .level = level,
                                       .build_cost = 600.0,
                                       .maintenance_cost = 8.5,
                                       .eff_mult = 1.44,
                                       .min_input = 5.0,
                                       .max_input = 10.0,
                                       .waste_mult = 0.42};
            default:
                return PowerPlantStats{};
            }

        case PlantType::GasPlant:
            switch (level)
            {
            case 1:
                return PowerPlantStats{.type = PlantType::GasPlant,
                                       .level = level,
                                       .build_cost = 65.0,
                                       .maintenance_cost = 4.0,
                                       .eff_mult = 1.15,
                                       .min_input = 4.0,
                                       .max_input = 8.0,
                                       .waste_mult = 0.5};
            case 2:
                return PowerPlantStats{.type = PlantType::GasPlant,
                                       .level = level,
                                       .build_cost = 320.0,
                                       .maintenance_cost = 5.2,
                                       .eff_mult = 1.61,
                                       .min_input = 4.0,
                                       .max_input = 8.0,
                                       .waste_mult = 0.425};
            case 3:
                return PowerPlantStats{.type = PlantType::GasPlant,
                                       .level = level,
                                       .build_cost = 900.0,
                                       .maintenance_cost = 6.8,
                                       .eff_mult = 2.07,
                                       .min_input = 4.0,
                                       .max_input = 8.0,
                                       .waste_mult = 0.35};
            default:
                return PowerPlantStats{};
            }

        case PlantType::BiomassPlant:
            switch (level)
            {
            case 1:
                return PowerPlantStats{.type = PlantType::BiomassPlant,
                                       .level = level,
                                       .build_cost = 90.0,
                                       .maintenance_cost = 6.0,
                                       .eff_mult = 0.75,
                                       .min_input = 3.0,
                                       .max_input = 6.0,
                                       .waste_mult = 0.4};
            case 2:
                return PowerPlantStats{.type = PlantType::BiomassPlant,
                                       .level = level,
                                       .build_cost = 450.0,
                                       .maintenance_cost = 7.8,
                                       .eff_mult = 1.05,
                                       .min_input = 3.0,
                                       .max_input = 6.0,
                                       .waste_mult = 0.34};
            case 3:
                return PowerPlantStats{.type = PlantType::BiomassPlant,
                                       .level = level,
                                       .build_cost = 1250.0,
                                       .maintenance_cost = 10.2,
                                       .eff_mult = 1.35,
                                       .min_input = 3.0,
                                       .max_input = 6.0,
                                       .waste_mult = 0.28};
            default:
                return PowerPlantStats{};
            }

        case PlantType::SolarPlant:
            switch (level)
            {
            case 1:
                return PowerPlantStats{.type = PlantType::SolarPlant,
                                       .level = level,
                                       .build_cost = 120.0,
                                       .maintenance_cost = 1.5,
                                       .eff_mult = 1.0,
                                       .min_input = 2.0,
                                       .max_input = 8.0,
                                       .waste_mult = 0.0};
            case 2:
                return PowerPlantStats{.type = PlantType::SolarPlant,
                                       .level = level,
                                       .build_cost = 720.0,
                                       .maintenance_cost = 1.95,
                                       .eff_mult = 1.4,
                                       .min_input = 2.0,
                                       .max_input = 8.0,
                                       .waste_mult = 0.0};
            case 3:
                return PowerPlantStats{.type = PlantType::SolarPlant,
                                       .level = level,
                                       .build_cost = 1900.0,
                                       .maintenance_cost = 2.55,
                                       .eff_mult = 1.8,
                                       .min_input = 2.0,
                                       .max_input = 8.0,
                                       .waste_mult = 0.0};
            default:
                return PowerPlantStats{};
            }

        case PlantType::WindPlant:
            switch (level)
            {
            case 1:
                return PowerPlantStats{.type = PlantType::WindPlant,
                                       .level = level,
                                       .build_cost = 150.0,
                                       .maintenance_cost = 2.5,
                                       .eff_mult = 1.05,
                                       .min_input = 3.0,
                                       .max_input = 9.0,
                                       .waste_mult = 0.0};
            case 2:
                return PowerPlantStats{.type = PlantType::WindPlant,
                                       .level = level,
                                       .build_cost = 900.0,
                                       .maintenance_cost = 3.25,
                                       .eff_mult = 1.47,
                                       .min_input = 3.0,
                                       .max_input = 9.0,
                                       .waste_mult = 0.0};
            case 3:
                return PowerPlantStats{.type = PlantType::WindPlant,
                                       .level = level,
                                       .build_cost = 2400.0,
                                       .maintenance_cost = 4.25,
                                       .eff_mult = 1.89,
                                       .min_input = 3.0,
                                       .max_input = 9.0,
                                       .waste_mult = 0.0};
            default:
                return PowerPlantStats{};
            }

        case PlantType::HydroPlant:
            switch (level)
            {
            case 1:
                return PowerPlantStats{.type = PlantType::HydroPlant,
                                       .level = level,
                                       .build_cost = 180.0,
                                       .maintenance_cost = 3.0,
                                       .eff_mult = 0.35,
                                       .min_input = 8.0,
                                       .max_input = 15.0,
                                       .waste_mult = 0.1};
            case 2:
                return PowerPlantStats{.type = PlantType::HydroPlant,
                                       .level = level,
                                       .build_cost = 1000.0,
                                       .maintenance_cost = 3.9,
                                       .eff_mult = 0.49,
                                       .min_input = 8.0,
                                       .max_input = 15.0,
                                       .waste_mult = 0.085};
            case 3:
                return PowerPlantStats{.type = PlantType::HydroPlant,
                                       .level = level,
                                       .build_cost = 2700.0,
                                       .maintenance_cost = 5.1,
                                       .eff_mult = 0.63,
                                       .min_input = 8.0,
                                       .max_input = 15.0,
                                       .waste_mult = 0.07};
            default:
                return PowerPlantStats{};
            }

        case PlantType::NuclearPlant:
            switch (level)
            {
            case 1:
                return PowerPlantStats{.type = PlantType::NuclearPlant,
                                       .level = level,
                                       .build_cost = 1200.0,
                                       .maintenance_cost = 18.0,
                                       .eff_mult = 2.5,
                                       .min_input = 1.0,
                                       .max_input = 2.0,
                                       .waste_mult = 4.0};
            case 2:
                return PowerPlantStats{.type = PlantType::NuclearPlant,
                                       .level = level,
                                       .build_cost = 8000.0,
                                       .maintenance_cost = 23.4,
                                       .eff_mult = 3.5,
                                       .min_input = 1.0,
                                       .max_input = 2.0,
                                       .waste_mult = 3.4};
            case 3:
                return PowerPlantStats{.type = PlantType::NuclearPlant,
                                       .level = level,
                                       .build_cost = 22000.0,
                                       .maintenance_cost = 30.6,
                                       .eff_mult = 4.5,
                                       .min_input = 1.0,
                                       .max_input = 2.0,
                                       .waste_mult = 2.8};
            default:
                return PowerPlantStats{};
            }

        default:
            return PowerPlantStats{};
        }
    }

    // Data for ResourcePlant Node & Building in each type+level
    ResourcePlantStats GetResourcePlantStats(ResourceType type, int purity_level)
    {
        switch (type)
        {
        case ResourceType::Coal:
            switch (purity_level)
            {
            case 1:
                return ResourcePlantStats{.type = ResourceType::Coal,
                                          .build_cost = 120.0,
                                          .maintenance_cost = 6.0,
                                          .product_per_turn = 10.0,
                                          .purity_level = purity_level,
                                          .waste_output = 16.0,
                                          .explore_cost = 25.0};
            case 2:
                return ResourcePlantStats{.type = ResourceType::Coal,
                                          .build_cost = 120.0,
                                          .maintenance_cost = 6.0,
                                          .product_per_turn = 10.0,
                                          .purity_level = purity_level,
                                          .waste_output = 13.0,
                                          .explore_cost = 25.0};
            case 3:
                return ResourcePlantStats{.type = ResourceType::Coal,
                                          .build_cost = 120.0,
                                          .maintenance_cost = 6.0,
                                          .product_per_turn = 10.0,
                                          .purity_level = purity_level,
                                          .waste_output = 10.0,
                                          .explore_cost = 25.0};
            default:
                return ResourcePlantStats{};
            }

        case ResourceType::Gas:
            switch (purity_level)
            {
            case 1:
                return ResourcePlantStats{.type = ResourceType::Gas,
                                          .build_cost = 250.0,
                                          .maintenance_cost = 6.0,
                                          .product_per_turn = 8.0,
                                          .purity_level = purity_level,
                                          .waste_output = 7.2,
                                          .explore_cost = 40.0};
            case 2:
                return ResourcePlantStats{.type = ResourceType::Gas,
                                          .build_cost = 250.0,
                                          .maintenance_cost = 6.0,
                                          .product_per_turn = 8.0,
                                          .purity_level = purity_level,
                                          .waste_output = 6.0,
                                          .explore_cost = 40.0};
            case 3:
                return ResourcePlantStats{.type = ResourceType::Gas,
                                          .build_cost = 250.0,
                                          .maintenance_cost = 6.0,
                                          .product_per_turn = 8.0,
                                          .purity_level = purity_level,
                                          .waste_output = 4.8,
                                          .explore_cost = 40.0};
            default:
                return ResourcePlantStats{};
            }

        case ResourceType::Biomass:
            switch (purity_level)
            {
            case 1:
                return ResourcePlantStats{.type = ResourceType::Biomass,
                                          .build_cost = 180.0,
                                          .maintenance_cost = 5.0,
                                          .product_per_turn = 6.0,
                                          .purity_level = purity_level,
                                          .waste_output = 7.5,
                                          .explore_cost = 50.0};
            case 2:
                return ResourcePlantStats{.type = ResourceType::Biomass,
                                          .build_cost = 180.0,
                                          .maintenance_cost = 5.0,
                                          .product_per_turn = 6.0,
                                          .purity_level = purity_level,
                                          .waste_output = 6.0,
                                          .explore_cost = 50.0};
            case 3:
                return ResourcePlantStats{.type = ResourceType::Biomass,
                                          .build_cost = 180.0,
                                          .maintenance_cost = 5.0,
                                          .product_per_turn = 6.0,
                                          .purity_level = purity_level,
                                          .waste_output = 4.5,
                                          .explore_cost = 50.0};
            default:
                return ResourcePlantStats{};
            }

        case ResourceType::Uranium:
            switch (purity_level)
            {
            case 1:
                return ResourcePlantStats{.type = ResourceType::Uranium,
                                          .build_cost = 3000.0,
                                          .maintenance_cost = 8.0,
                                          .product_per_turn = 8.0,
                                          .purity_level = purity_level,
                                          .waste_output = 3.6,
                                          .explore_cost = 150.0};
            case 2:
                return ResourcePlantStats{.type = ResourceType::Uranium,
                                          .build_cost = 3000.0,
                                          .maintenance_cost = 8.0,
                                          .product_per_turn = 8.0,
                                          .purity_level = purity_level,
                                          .waste_output = 3.2,
                                          .explore_cost = 150.0};
            case 3:
                return ResourcePlantStats{.type = ResourceType::Uranium,
                                          .build_cost = 3000.0,
                                          .maintenance_cost = 8.0,
                                          .product_per_turn = 8.0,
                                          .purity_level = purity_level,
                                          .waste_output = 2.8,
                                          .explore_cost = 150.0};
            default:
                return ResourcePlantStats{};
            }

        default:
            return ResourcePlantStats{};
        }
    }

    // Create City Data for Add in Node
    CityData CreateCityData(CityType type)
    {
        double min_Energy = 0.0;
        double max_Energy = 0.0;
        double Elec_Charge = 0.0;
        if (type == CityType::Small)
        {
            min_Energy = 150.0;
            max_Energy = 300.0;
            Elec_Charge = 1.5;
        }
        else if (type == CityType::Big)
        {
            min_Energy = 400.0;
            max_Energy = 600.0;
            Elec_Charge = 2.0;
        }
        return {type, min_Energy, max_Energy, Elec_Charge};
    }

    // Send Rate of Calculate Shipping Cost
    double GetTranportRate(TransportType item)
    {
        switch (item)
        {
        case TransportType::Resource:
            return 0.5;
        case TransportType::Energy:
            return 0.2;
        default:
            return 0;
        }
    }

} // namespace GameData
