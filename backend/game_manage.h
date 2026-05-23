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
extern std::vector<Link *> all_links_;

Node* GetOrCreateNode(int r, int c, std::string type, std::string subtype);
Player* GetCurrentPlayer();
bool NextTurn();
void ProcessWorld();

    
//End Game Logic
bool isEndGame();
bool IsPlayerLost(const Player* player); //check and delete player from game
Player* GetWinner();
