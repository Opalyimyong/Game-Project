#pragma once

//Base Data

class Player;

enum class SourceType {
    Resource_Based,
    Passive,
    None
};

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
    float min_Energy;
    float max_Energy;
    float Elec_Charge; 
};

struct ResourcePlantStats {
    ResourceType type;
    float build_cost;
    float maintenance_cost;
    float product_per_turn;
    int purity_level;
    float waste_output;
    float explore_cost;
};

struct PowerPlantStats {
    PlantType type;
    int level;
    float build_cost;
    float maintenance_cost;

    //Eff Multiplier
    float eff_mult;

    float min_input;
    float max_input;

    //Waste Multiplier
    float waste_mult;
};

struct Item { //Item sending type For communicate Link & Building
    TransportType type;
    float amount;
};

struct CityContract {
    Player* player;
    float amount_recieved;
};
