#pragma once

//Base

enum class ResourceType {
    Coal, Gas, Biomass, Uranium
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
    int purity_level;
    double waste_output;
    double explore_cost;
};

struct PowerPlantStats {
    PlantType type;
    int level;
    double build_cost;
    double maintenance_cost;

    //Eff Multiplier
    double eff_mult;

    double min_input;
    double max_input;

    //Waste Multiplier
    double waste_mult;
};

struct CityContract { //WHAT IS THIS!!
    int player_id;
    double amount_reserved;
    double amount_sended;
};
