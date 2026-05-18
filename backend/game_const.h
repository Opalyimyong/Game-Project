#pragma once

// Base Data

class Player;

enum class SourceType
{
    Resource_Based,
    Passive,
    None
};

enum class ResourceType
{
    Coal,
    Gas,
    Biomass,
    Uranium
};

enum class PlantType
{
    CoalPlant,
    GasPlant,
    BiomassPlant,
    SolarPlant,
    WindPlant,
    HydroPlant,
    NuclearPlant
};

enum class CityType
{
    Small,
    Big
};

enum class TransportType
{
    Resource,
    Energy
};

struct CityData
{
    CityType type;
    double min_Energy;
    double max_Energy;
    double Elec_Charge;
};

struct ResourcePlantStats
{
    ResourceType type;
    double build_cost;
    double maintenance_cost;
    double product_per_turn;
    int purity_level;
    double waste_output;
    double explore_cost;
};

struct PowerPlantStats
{
    PlantType type;
    int level;
    double build_cost;
    double maintenance_cost;

    // Eff Multiplier
    double eff_mult;

    double min_input;
    double max_input;

    // Waste Multiplier
    double waste_mult;
};

struct Item
{ // Item sending type For communicate Link & Building
    TransportType type;
    double amount;
};

struct CityContract
{
    Player *player;
    double amount_recieved;
};
