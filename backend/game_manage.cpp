#include "game_manage.h"

GameManager::GameManager(std::vector<Player*> players) : players_(players) {}

Player* GameManager::GetCurrentPlayer() const
{
    //implement
}

void GameManager::NextTurn()
{
    //implement
}

Player* GameManager::GetWinner() const
{
    //implement
}