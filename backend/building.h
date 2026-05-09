#pragma once
#include <vector>
#include <string>

#include "game_data.h"

class Player;

class Building {
protected:
    int id_;
    bool is_active_;
    int level_;
    Player* owner_;
    std::vector<int> location_node_;
public:
    // Contructor & Destructor
    Building(const int& id, Player* owner, const std::vector<int> location_node);
    virtual ~Building() = default;

    // Getter
    const int& getId() const { return id_; }
    bool IsActive() const { return is_active_; }
    int getLevel() const { return level_; }
    Player* getOwner() const { return owner_; }
    const std::vector<int>& getLocationNode() const { return location_node_; }

    // Setter
    bool toggleStatus(bool status);
    virtual bool upgrade() = 0; //need to overide
    virtual double process() = 0; //need to overide
    virtual int getCurrentValue() const = 0; //need to overide
};

class ResourcePlant : public Building {
private:
    ResourceType type_;
    double resource_;
    ResourcePlantStats stats_;
public:
    // Constructor
    ResourcePlant(const int& id, Player* owner, const std::vector<int> location_node, ResourceType type);
    
    // Getter
    ResourceType getType() const;
    double getStorage() const;

    // Setter
    double consumeStorage(double amount);
    bool upgrade() override; //Check Coin >> Upgrade Level >> Pay Coin
    double process() override; //Process Resorce & Waste by stats_ >> Add to Storage
    int getCurrentValue() const override;
};

class PowerPlant : public Building {
private:
    /* data */
public:
    PowerPlant
(/* args */);
    ~PowerPlant
();
};
