#include "player.h"
#include "building.h"
#include "node.h"
#include "game_data.h"

#include <iostream>

Player::Player(const std::string &id)
    : id_(id), coins_(500.0), AP_(3), waste_(0.0), buildings_({}) {}

bool Player::executeManualAction(double cost)
{
    if (AP_ < 1)
    {
        whileError_ = "Your Action Point Not Enought";
        return false;
    }
    if (coins_ - cost < 0.0)
    {
        whileError_ = "Your Coin Not Enought";
        return false;
    }

    // Action
    AP_ -= 1;
    coins_ -= cost;
    return true;
}

bool Player::payAutoRunCost(double cost)
{
    if (coins_ - cost < 0.0)
    {
        whileError_ = "Your Coin Not Enought";
        return false;
    }

    // Subscribtion
    coins_ -= cost;
    return true;
}

void Player::addCoins(double amount) { coins_ += amount; }

void Player::addWaste(double amount) { waste_ += amount; }

bool Player::disposeWaste(double amount)
{
    if (AP_ < 1)
    {
        whileError_ = "Your Action Point Not Enought";
        return false;
    }
    if (coins_ - (amount * 3.0) < 0.0)
    {
        whileError_ = "Your Coin Not Enought";
        return false;
    }

    // Action
    AP_ -= 1;
    coins_ -= (amount * 3.0);
    waste_ -= amount;
    if (waste_ < 0) {
        waste_ = 0.0;
    }
    return true;
}

double Player::getEfficiencyModifier() const
{
    if (waste_ >= 80.0)
    {
        return 0.8;
    }
    return 0.0;
}

void Player::resetActionPoints() { AP_ = 3; }

void Player::registerBuilding(Building *building) { buildings_.push_back(building); }

void Player::accessCityNode(CityNode *node) { city_nodes_.push_back(node); }

double Player::calculateAssetValue() const
{
    double Cpoint = coins_;
    double Bpoint = 0.0;
    double Npoint = 0.0;
    
    for (const auto &building : buildings_) { Bpoint += building->getValue(); } //cal all buidling value
    for (const auto &node : city_nodes_) {
        if (node->getCityData().type == CityType::Small) {
            Npoint += 75.0;
        }
        else if (node->getCityData().type == CityType::Big) {
            Npoint += 150.0;
        }
    }
    double waste_penalty = waste_ * 2.0; // แต่ละหน่วยของขยะจะลดค่าโดยรวมลง 2 หน่วย

    return Cpoint + Bpoint + Npoint - waste_penalty;
}

bool Player::isBankrupt() const
{
    if (coins_ < 0.0)
    {
        return true;
    }
    return false;
}

bool Player::isGameOver() const
{
    {
        if (isBankrupt() || waste_ >= 100.0)
        {
            return true;
        }
    }
    return false;
}
