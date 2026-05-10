#include "building.h"
#include "player.h"

// Building Class
Building::Building(const int &id, Player *owner, const std::vector<int> location_node, Item item)
    : id_(id), is_active_(true), level_(1), owner_(owner), location_node_(location_node), item_(item) {}

bool Building::toggleStatus(bool status)
{
    if ((is_active_ == true) and (status == false))
    {
        is_active_ = false;
        return true;
    }
    if ((is_active_ == false) && (status == true))
    {
        if ((owner_ != nullptr) && (owner_->executeManualAction(0)))
        {
            is_active_ = true;
            return true;
        }
    }
    return false;
}

// ResourcePlant Class
ResourcePlant::ResourcePlant(const int &id, Player *owner, const std::vector<int> location_node, ResourceType type, int purity_level)
    : Building(id, owner, location_node, {TransportType::Resource, 0.0}), type_(type), purity_level_(purity_level)
{
    level_ = purity_level_;
    stats_ = GameData::GetResourcePlantStats(type, level_);
}

bool ResourcePlant::upgrade() { return false; } // Resource Plant cannot upgrade

void ResourcePlant::processWaste()
{
    double waste = stats_.waste_output;
    waste_output_ = waste; // per turn waste output
}

bool ResourcePlant::process()
{
    item_.amount = stats_.product_per_turn;
    ResourcePlant::processWaste();
    return true;
}

int ResourcePlant::getCurrentValue() const
{ // 70% of Build Cost
    return static_cast<int>(stats_.build_cost * 70 / 100);
}

// PowerPlant Class
PowerPlant::PowerPlant(const int &id, Player *owner, const std::vector<int> location_node, PlantType type)
    : Building(id, owner, location_node, {TransportType::Energy, 0.0}), type_(type)
{
    stats_ = GameData::GetPowerPlantStats(type, level_);
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
        passive_type_ = PassiveType::Solar;
        break;
    case PlantType::WindPlant:
        source_type_ = SourceType::Passive;
        passive_type_ = PassiveType::Wind;
        break;
    case PlantType::HydroPlant:
        source_type_ = SourceType::Passive;
        passive_type_ = PassiveType::Water;
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

void PowerPlant::clearResourceInput() { resource_input_ = {TransportType::Resource, 0.0}; }

bool PowerPlant::upgrade()
{
    if (level_ < 3)
    {
        int next_level = level_ + 1;
        PowerPlantStats next_stats = GameData::GetPowerPlantStats(type_, next_level);
        double upgrade_cost = next_stats.build_cost;
        if (owner_ != nullptr and owner_->executeManualAction(upgrade_cost))
        {
            level_ = next_level;
            stats_ = next_stats;
            return true;
        }
    }
    return false;
}

void PowerPlant::processWaste()
{
    double waste = resource_input_.amount * stats_.waste_mult;
    waste_output_ = waste; // per turn waste output
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

int PowerPlant::getCurrentValue() const
{ // 75% of Build Cost
    return static_cast<int>(stats_.build_cost * 75 / 100);
}
