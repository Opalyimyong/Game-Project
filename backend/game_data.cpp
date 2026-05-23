#include "game_data.h"

namespace GameData
{

    // Data for PowerPlant Node & Building in each type+level
    PowerPlantStats GetPowerPlantStats(PlantType type)
    {
        switch (type)
        {
        case PlantType::CoalPlant:
            return PowerPlantStats{.type = PlantType::CoalPlant,
                                        .build_cost = 80.0,
                                        .eff_mult = 0.8,
                                        .min_input = 5.0,
                                        .max_input = 10.0,
                                        .waste_mult = 0.6};

        case PlantType::GasPlant:
            return PowerPlantStats{.type = PlantType::GasPlant,
                                        .build_cost = 140.0,
                                        .eff_mult = 1.15,
                                        .min_input = 4.0,
                                        .max_input = 8.0,
                                        .waste_mult = 0.5};

        case PlantType::BiomassPlant:
                return PowerPlantStats{.type = PlantType::BiomassPlant,
                                        .build_cost = 120.0,
                                        .eff_mult = 0.75,
                                        .min_input = 3.0,
                                        .max_input = 6.0,
                                        .waste_mult = 0.4};
        case PlantType::SolarPlant:
            return PowerPlantStats{.type = PlantType::SolarPlant,
                                        .build_cost = 160.0,
                                        .eff_mult = 1.0,
                                        .min_input = 2.0,
                                        .max_input = 8.0,
                                        .waste_mult = 0.0};
        case PlantType::WindPlant:
            return PowerPlantStats{.type = PlantType::WindPlant,
                                        .build_cost = 180.0,
                                        .eff_mult = 1.05,
                                        .min_input = 3.0,
                                        .max_input = 9.0,
                                        .waste_mult = 0.0};
        case PlantType::HydroPlant:
            return PowerPlantStats{.type = PlantType::HydroPlant,
                                        .build_cost = 250.0,
                                        .eff_mult = 0.35,
                                        .min_input = 8.0,
                                        .max_input = 15.0,
                                        .waste_mult = 0.1};
        case PlantType::NuclearPlant:
            return PowerPlantStats{.type = PlantType::NuclearPlant,
                                        .build_cost = 1200.0,
                                        .eff_mult = 2.5,
                                        .min_input = 1.0,
                                        .max_input = 2.0,
                                        .waste_mult = 4.0};
        default:
            return PowerPlantStats{};
        }
    }

    // Data for ResourcePlant Node & Building in each type+level
    ResourcePlantStats GetResourcePlantStats(ResourceType type)
    {
        switch (type)
        {
        case ResourceType::Coal:
            return ResourcePlantStats{.type = ResourceType::Coal,
                                          .build_cost = 100.0,
                                          .product_per_turn = 10.0,
                                          .waste_output = 8.0,};

        case ResourceType::Gas:
            return ResourcePlantStats{.type = ResourceType::Gas,
                                          .build_cost = 160.0,
                                          .product_per_turn = 8.0,
                                          .waste_output = 4.8,};

        case ResourceType::Biomass:
            return ResourcePlantStats{.type = ResourceType::Biomass,
                                          .build_cost = 180.0,
                                          .product_per_turn = 6.0,
                                          .waste_output = 3.0,};    

        case ResourceType::Uranium:
            return ResourcePlantStats{.type = ResourceType::Uranium,
                                          .build_cost = 500.0,
                                          .product_per_turn = 2.0,
                                          .waste_output = 1.8,};

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
            min_Energy = 10.0;
            max_Energy = 20.0;
            Elec_Charge = 2.0;
        }
        else if (type == CityType::Big)
        {
            min_Energy = 25.0;
            max_Energy = 40.0;
            Elec_Charge = 2.5;
        }
        return {type, min_Energy, max_Energy, Elec_Charge};
    }

    // Send Rate of Calculate Shipping Cost
    double GetTranportRate(TransportType item)
    {
        switch (item)
        {
        case TransportType::Resource:
            return 2.0;
        case TransportType::Energy:
            return 1.0;
        default:
            return 0;
        }
    }

} // namespace GameData
