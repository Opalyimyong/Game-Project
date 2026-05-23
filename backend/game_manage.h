#pragma once

#include <string>
#include <vector>

#include "game_data.h"
#include "player.h"
#include "link.h"

//parameter for game manage
extern std::vector<Player *> players_;
extern int turn_index_;
extern std::vector<Node *> all_nodes_;

Player* GetCurrentPlayer();
void NextTurn();
    
//End Game Logic
bool isEndGame();
bool IsPlayerLost(const Player* player); //check and delete player from game
Player* GetWinner();
