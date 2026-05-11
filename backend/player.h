#pragma once
#include <vector>
#include <string>

class Building;

class Player {
    private:
        std::string id_;
        float coins_; //unit = kCoins
        int AP_; // Action Point 3 points per turn
        float waste_; // > 100
        std::vector<Building*> buildings_; //collection of building that player have
        std::string whileError_ = "";
    public:
        // Constructor & Destructor
        Player(const std::string& id);
        virtual ~Player() = default;

        // Getter
        const std::string& getId() const { return id_; }
        float getCoins() const { return coins_; }
        int getActionPoints() const { return AP_; }
        float getTotalWaste() const { return waste_; }

        // Setter
        bool executeManualAction(float cost); //Use Action + Use Coin
        bool payAutoRunCost(float cost); //Subscription
        void addCoins(float amount);
        void addWaste(float amount);
        bool disposeWaste(float amount); //check money >> delete waste >> executeAction
        float getEfficiencyModifier() const;
        void resetActionPoints();
        void registerBuilding(Building* building);  //add building to vec buildings_
        float calculateAssetValue() const; //cal all value
        bool isBankrupt() const;
        bool isGameOver() const;
};