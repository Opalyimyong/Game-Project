#include <vector>
#include <string>

class Building;

class Player {
    private:
        std::string id_;
        double coins_; //unit = kCoins
        int action_points_; // 3 points per turn
        double total_waste_; // > 100

};