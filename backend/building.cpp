#include "building.h"
#include "player.h"

// Building Class
Building::Building(const int &id, BuildingType building_type, Player *owner, const std::vector<int> location_node, Item item)
    : id_(id), is_active_(true), level_(1), building_type_(building_type), owner_(owner), location_node_(location_node), item_(item) {}

// ResourcePlant Class
ResourcePlant::ResourcePlant(const int &id, Player *owner, const std::vector<int> location_node, ResourceType type, int purity_level)
    : Building(id, BuildingType::ResourcePlant, owner, location_node, {TransportType::Resource, 0.0}), type_(type), purity_level_(purity_level)
{
    level_ = purity_level_;
    stats_ = GameData::GetResourcePlantStats(type);
}

void ResourcePlant::processWaste()
{
    double waste = stats_.waste_output;
    item_.waste_amount += waste; // per turn waste output
}

bool ResourcePlant::process()
{
    item_.amount = stats_.product_per_turn;
    ResourcePlant::processWaste();
    return true;
}

void ResourcePlant::setCurrentValue()
{
    value_ = stats_.build_cost * 70.0 / 100.0;
}

// PowerPlant Class
PowerPlant::PowerPlant(const int &id, Player *owner, const std::vector<int> location_node, PlantType type)
    : Building(id, BuildingType::PowerPlant, owner, location_node, {TransportType::Energy, 0.0}), type_(type)
{
    stats_ = GameData::GetPowerPlantStats(type);
    switch (type_)
    {
    case PlantType::CoalPlant:
        source_type_ = SourceType::Resource_Based;
        break;
    case PlantType::GasPlant:
        source_type_ = SourceType::Resource_Based;
        break;
    case PlantType::BiomassPlant:
        source_type_ = SourceType::Resource_Based;
        break;
    case PlantType::SolarPlant:
        source_type_ = SourceType::Passive;
        break;
    case PlantType::WindPlant:
        source_type_ = SourceType::Passive;
        break;
    case PlantType::HydroPlant:
        source_type_ = SourceType::Passive;
        break;
    case PlantType::NuclearPlant:
        source_type_ = SourceType::Resource_Based;
        break;
    default:
        break;
    }
}

bool PowerPlant::addResourceInput(Item input)
{
    if (input.type == TransportType::Resource)
    {
        resource_input_ = input;
        return true;
    }
    return false;
}

void PowerPlant::clearResourceInput() { resource_input_ = {TransportType::Resource, 0.0f}; }

void PowerPlant::processWaste()
{
    float waste = resource_input_.amount * stats_.waste_mult;
    item_.waste_amount += waste; // per turn waste output
}

bool PowerPlant::process()
{
    if (resource_input_.amount < stats_.min_input)
    {
        return false; // Resource input not in range
    }
    if (resource_input_.amount > stats_.max_input)
    {
        resource_input_.amount = stats_.max_input; // Resource input above max, use max input
    }
    item_.amount = resource_input_.amount * stats_.eff_mult;
    PowerPlant::processWaste();
    return true;
}

void PowerPlant::setCurrentValue()
{
    value_ = stats_.build_cost * 75.0f / 100.0f;
}
