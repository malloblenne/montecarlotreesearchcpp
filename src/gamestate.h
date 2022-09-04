#pragma once

#include "action.h"

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

namespace mctsearch
{
    enum class WinningState
    {
        LOSS = -1,
        DRAW = 0,
        WIN = 1
    };
    
    double winningstate_to_double(WinningState ws)
    {
        switch(ws)
        {
            case WinningState::LOSS: return -1.0;
            case WinningState::DRAW: return 0.0;
            case WinningState::WIN: return 1.0;
            // we enumerate all and compiler will warn/error otherwise
            // no need of default
        }
        throw std::domain_error("WinningState undefined in switch case");
    }

    class GameState
    {
        public:
        virtual ~GameState() = default;

        virtual std::string to_string() const = 0;

        virtual int number_moves() = 0;

    	virtual void apply_nth_move(int n) = 0;
	
    	virtual int get_owner() const = 0;

        virtual bool is_terminal() const = 0;

        virtual std::vector<std::string> drawables() const = 0;

        virtual WinningState score(int player) const = 0;

        virtual std::unique_ptr<GameState> clone() const = 0;

        virtual std::unique_ptr<Action> get_action_nth(int n) const = 0;


    };
}