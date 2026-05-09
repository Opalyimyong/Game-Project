#pragma once

#include <string>
#include <vector>

enum class ActionType {
    BuildPowerPlant,
    BuildResourcePlant,
    CreateLink,
    RemoveWaste
};

class Player {
public:
    Player(const std::string& name) : name_(name), action_points_(3) {}

    const std::string& GetName() const { return name_; }
    int GetActionPoints() const { return action_points_; }

    bool CanPerformAction() const { return action_points_ > 0; }
    void SpendActionPoint() { if (action_points_ > 0) action_points_--; }
    void ResetActionPoints() { action_points_ = 3; }

private:
    std::string name_;
    int action_points_;
};

class GameState {
public:
    GameState() : current_player_index_(0) {}

    void AddPlayer(const Player& player) { players_.push_back(player); }
    Player& GetCurrentPlayer() { return players_[current_player_index_]; }
    const std::vector<Player>& GetPlayers() const { return players_; }

    void NextTurn() {
        players_[current_player_index_].ResetActionPoints();
        current_player_index_ = (current_player_index_ + 1) % players_.size();
    }

    bool IsGameOver() const { return false; } // TODO: implement win conditions

private:
    std::vector<Player> players_;
    size_t current_player_index_;
};