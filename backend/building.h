#pragma once
#include <vector>
#include <string>

#include "game_data.h"

class Player;

class Building
{
protected:
    int id_;
    bool is_active_;
    int level_;
    BuildingType building_type_;
    Player *owner_; // prt to class Player for using function in Player
    std::vector<int> location_node_;
    Item item_;                 // Product for send to node and link
    double waste_output_ = 0.0; // waste output per turn
    SourceType source_type_ = SourceType::None; // for input resource type of power plant
    double value_ = 0.0;             // for calculate asset value of player

public:
    // Contructor & Destructor
    Building(const int &id, BuildingType building_type, Player *owner, const std::vector<int> location_node, Item item);
    virtual ~Building() = default;
    virtual bool addResourceInput(Item input) { return false; } // for power plant to add resource input from link
    // Getter
    const int &getId() const { return id_; }
    bool IsActive() const { return is_active_; }
    int getLevel() const { return level_; }
    Player *getOwner() const { return owner_; }
    const std::vector<int> &getLocationNode() const { return location_node_; }
    double getWasteOutput() const { return waste_output_; }
    Item getItem() const { return item_; } // override for resource and energy >> Send This to Node and Link
    double getValue() const { return value_; }

    // Setter
    bool toggleStatus(bool status);          // in case player off plant and want to turn on again
    virtual bool upgrade() = 0;              // need to overide
    virtual void processWaste() = 0;         // add waste output to building's waste
    virtual bool process() = 0;              // process in plant and add waste output to building's waste
    virtual void setCurrentValue() = 0;     // set current value for calculate asset value of player
};

class ResourcePlant : public Building
{
private:
    ResourceType type_;
    ResourcePlantStats stats_;
    int purity_level_;

public:
    // Constructor
    ResourcePlant(const int &id, Player *owner, const std::vector<int> location_node, ResourceType type, int purity_level);

    // Getter
    ResourceType getType() const { return type_; }

    // Setter
    bool upgrade() override;      // resource cannot upgrade
    void processWaste() override; // Process Waste by stats_ >> Add to Building's Waste
    bool process() override;      // Process Resorce & Waste by stats_ >> Add to Storage
    void setCurrentValue() override;
};

class PowerPlant : public Building
{
private:
    PlantType type_;
    Item resource_input_;
    PowerPlantStats stats_;
public:
    // Contructor & Destructor
    PowerPlant(const int &id, Player *owner, const std::vector<int> location_node, PlantType type);

    // Getter
    PlantType getType() const { return type_; }
    Item getResourceInput() const { return resource_input_; }
    SourceType getSourceType() const { return source_type_; }
    // Setter
    bool addResourceInput(Item input) override; // input resource from outter
    void clearResourceInput();         // at end of process

    bool upgrade() override;      // Check Coin >> Upgrade Level >> Pay Coin
    void processWaste() override; // Process Waste by stats_ >> Add to Building's Waste
    bool process() override;      // Process Resorce & Waste by stats_ >> Add to Storage
    void setCurrentValue() override;
};
