#pragma once

enum class ResourceType {
    Coal, Gas, Biomass, Uramiun
};

enum class PlantType {
    CoalPlant, GasPlant, BiomassPlant, SolarPlant, WindPlant, HydroPlant, NuclearPlant
};

enum class CityType {
    Small, Big
};

enum class TransportType {
    Resource, Energy
};

struct CityData {
    CityType type;
    double min_ENGdemand;
    double max_ENGdemand;
    double Elec_Charge; 
};

struct ResourcePlantStats {
    ResourceType type;
    double build_cost;
    double maintenance_cost;
    double product_per_turn;
    double purity_lv1_mult;
    double purity_lv2_mult;
    double purity_lv3_mult;
    double explore_cost;
};

struct PowerPlantStats {
    PlantType type;
    int level;
    double build_cost;
    double maintenance_cost;

    //Eff Multiplier
    double eff_type_mult;
    double eff_lv1_mult = 1.0;
    double eff_lv2_mult = 1.4;
    double eff_lv3_mult = 1.8;
    //Total Eff = type*lv

    double min_input;
    double max_input;

    //Waste Multiplier
    double waste_type_mult;
    double waste_lv1_mult = 1.0;
    double waste_lv2_mult = 0.85;
    double waste_lv3_mult = 0.7;
    //Total Waste = type*lv
};

struct CityContract { //WHAT IS THIS!!
    int player_id;
    double amount_reserved;
    double amount_sended;
};
