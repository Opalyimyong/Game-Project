#pragma once

#include <string>
#include <vector>

#include "game_data.h"
#include "player.h"
#include "link.h"

class GameManager {
public:
    GameManager(std::vector<Player*> players);

    //Getter
    Player* GetCurrentPlayer() const;
    void NextTurn();
    
    //End Game Logic
    bool EndGame() const;
    bool IsPlayerBankrupt(const Player* player) const; //check and delete player from game
    Player* GetWinner() const;


private:
    std::vector<Player *> players_;
    int turn_index_ = 0;

};