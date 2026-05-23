#pragma once

#include <string>
#include <vector>

#include "game_data.h"
#include "player.h"
#include "link.h"

//parameter for game manage
std::vector<Player *> players_ = {};
int turn_index_ = 0;

Player* GetCurrentPlayer();
void NextTurn();
    
//End Game Logic
bool isEndGame();
bool IsPlayerLoss(const Player* player); //check and delete player from game
Player* GetWinner();
