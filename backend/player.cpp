#include "player.h"
#include "building.h"

Player::Player(const std::string& id)
: id_(id), coins_(500), AP_(3), waste_(0), buildings_({}) {}

bool Player::executeManualAction(double cost)
{
    return false;
}
