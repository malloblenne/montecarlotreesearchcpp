#pragma once

#include "gamestate.h"

class TestAction: public mctsearch::Action
{
public:
    explicit TestAction(int index): index_(index) {}
    std::string to_string() const override { return std::to_string(index_); }
    int value() const override { return index_; }

private:
    int index_;
};

class TestGameState: public mctsearch::GameState
{
public:
    explicit TestGameState(int remaining = 2, int choices = 1):
        remaining_(remaining), choices_(choices) {}

    std::string to_string() const override { return std::to_string(remaining_); }
    int number_moves() override { return is_terminal() ? 0 : choices_; }
    void apply_nth_move(int index) override
    {
        if (is_terminal() || index < 0 || index >= choices_)
        {
            throw std::invalid_argument("Invalid test move");
        }
        --remaining_;
        owner_ = owner_ == 1 ? 2 : 1;
    }
    int get_owner() const override { return owner_; }
    bool is_terminal() const override { return remaining_ == 0; }
    std::vector<std::string> drawables() const override { return {}; }
    mctsearch::WinningState score(int player) const override
    {
        if (!is_terminal())
        {
            return mctsearch::WinningState::DRAW;
        }
        return player == 1 ? mctsearch::WinningState::WIN : mctsearch::WinningState::LOSS;
    }
    std::unique_ptr<mctsearch::GameState> clone() const override
    {
        return std::make_unique<TestGameState>(*this);
    }
    std::unique_ptr<mctsearch::Action> get_action_nth(int index) const override
    {
        if (is_terminal() || index < 0 || index >= choices_)
        {
            throw std::invalid_argument("Invalid test action");
        }
        return std::make_unique<TestAction>(index);
    }

private:
    int remaining_;
    int choices_;
    int owner_ = 1;
};