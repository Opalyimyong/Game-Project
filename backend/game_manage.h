#pragma once

#include <string>
#include <vector>

#include "game_data.h"
#include "player.h"
#include "link.h"

class GameManager {
public:
    GameManager(std::vector<Player*> players);
    Player* GetCurrentPlayer() const;
    void NextTurn();
    

private:
    std::vector<Player *> players_;
    int turn_index_ = 0;

};