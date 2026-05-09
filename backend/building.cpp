#include "building.h"
#include "player.h"

//
Building::Building(const int& id, Player* owner, const std::vector<int> location_node) 
    : id_(id), is_active_(true), level_(1), owner_(owner), location_node_(location_node) {}

bool Building::toggleStatus(bool status) {
    
}