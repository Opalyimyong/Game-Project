#pragma once

#include <string>
#include <vector>

#include "game_data.h"
#include "player.h"
#include "link.h"

class GameManager {
public:
    GameManager();
    void update();
    void addPlayer(Player *player);
    void addLink(Link *link);
    std::vector<Player *> getPlayers() const;
    std::vector<Link *> getLinks() const;

private:
    std::vector<Player *> players_;
    std::vector<Node *> nodes_;
    std::vector<Link *> links_;
};