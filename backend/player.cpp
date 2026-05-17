// #include "player.h"
// #include "building.h"

// #include <iostream>

// Player::Player(const std::string& id)
// : id_(id), coins_(500.0f), AP_(3), waste_(0.0f), buildings_({}) {}

// bool Player::executeManualAction(float cost) {
//     if (AP_ < 1) {
//         whileError_ = "Your Action Point Not Enought";
//         return false;
//     }
//     if (coins_ - cost < 0.0f) {
//         whileError_ = "Your Coin Not Enought";
//         return false;
//     }

//     //Action
//     AP_ -= 1;
//     coins_ -= cost;
//     return true;
// }

// bool Player::payAutoRunCost(float cost)
// {
//     if (coins_ - cost < 0.0f) {
//         whileError_ = "Your Coin Not Enought";
//         return false;
//     }

//     //Subscribtion
//     coins_ -= cost;
//     return true;
// }

// void Player::addCoins(float amount) { coins_ += amount; }

// void Player::addWaste(float amount) { waste_ += amount; }

// bool Player::disposeWaste(float amount)
// {
//     if (coins_ - (amount * 3.0f) < 0.0f) {
//         whileError_ = "Your Coin Not Enought";
//         return false;
//     }

//     //Action
//     AP_ -= 1;
//     coins_ -= (amount * 3.0f);
//     return true;
// }

// float Player::getEfficiencyModifier() const
// {
//     if (waste_ >= 80.0f) {
//         return 0.8f;
//     }
//     return 0.0f;
// }

// void Player::resetActionPoints() { AP_ = 3; }

// void Player::registerBuilding(Building* building) { buildings_.push_back(building); }

// float Player::calculateAssetValue() const
// {
//     return 0; // COME BACK
// }

// bool Player::isBankrupt() const
// {
//     if (coins_ < 0.0f) { return true; }
//     return false;
// }

// bool Player::isGameOver() const
// {
//     {if (isBankrupt() || waste_ >= 100.0f) { return true; }}
//     return false;
// }
