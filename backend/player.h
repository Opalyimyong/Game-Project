#pragma once
#include <vector>
#include <string>

#include "Node.h"

class Building;

class Player
{
private:
    std::string id_;
    double coins_;                      // unit = kCoins
    int AP_;                            // Action Point 3 points per turn
    double waste_;                      // > 100
    std::vector<Building *> buildings_; // collection of building that player have
    std::string whileError_ = "";
    std::vector<CityNode *> city_nodes_; // collection of node that player have

public:
    // Constructor & Destructor
    Player(const std::string &id);
    virtual ~Player() = default;

    // Getter
    const std::string &getId() const { return id_; }
    double getCoins() const { return coins_; }
    int getActionPoints() const { return AP_; }
    double getTotalWaste() const { return waste_; }

    // Setter
    bool executeManualAction(double cost); // Use Action + Use Coin
    bool payAutoRunCost(double cost);      // Subscription
    void addCoins(double amount);
    void addWaste(double amount);
    bool disposeWaste(double amount); // check money >> delete waste >> executeAction
    double getEfficiencyModifier() const;
    void resetActionPoints();
    void registerBuilding(Building *building); // add building to vec buildings_
    void accessCityNode(CityNode *node); // add node to vec city_nodes_
    double calculateAssetValue() const;        // cal all value
    bool isBankrupt() const;
    bool isGameOver() const;
    bool isAllCityNodesUnpowered() const;
};