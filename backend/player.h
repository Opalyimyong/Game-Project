#include <vector>
#include <string>

class Building;

class Player {
    private:
        std::string id_;
        double coins_; //unit = kCoins
        int AP_; // Action Point 3 points per turn
        double waste_; // > 100
        std::vector<Building*> buildings_;
        std::string whileError_ = "";
    public:
        // Constructor & Destructor
        Player(const std::string& id);
        virtual ~Player() = default;

        // Getter
        const std::string& getId() const { return id_; }
        int getCoins() const { return coins_; }
        int getActionPoints() const { return AP_; }
        double getTotalWaste() const { return waste_; }
        bool isBankrupt() const {if (coins_ < 0.0) { return true; }}
        bool isGameOver() const {if (isBankrupt() or waste_ >= 100) { return true; }}

        // Setter
        bool executeManualAction(double cost); //Use Action + Use Coin
        bool payAutoRunCost(double cost); //Subscription
        void addCoins(double amount);
        void addWaste(double amount);
        bool disposeWaste(double amount); //check money >> delete waste >> executeAction
        double getEfficiencyModifier() const;
        void resetActionPoints();
        void registerBuilding(Building* building);  //add building to vec buildings_
        double calculateAssetValue() const; //cal all value
        bool isBankrupt() const;
        bool isGameOver() const;
};