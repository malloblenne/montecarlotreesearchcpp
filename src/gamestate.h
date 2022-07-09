#pragma once

#include <vector>
#include <string>

namespace mctsearch
{
    enum class WinningState
    {
        LOSS = -1,
        DRAW = 0,
        WIN = 1
    };

    class GameState
    {
        public:
        virtual std::string to_string() const = 0;

        virtual int number_moves() = 0;

    	virtual void apply_nth_move(int n) = 0;
	
    	virtual int get_owner() const = 0;

        virtual bool is_terminal() const = 0;

        virtual std::vector<std::string> drawables() const = 0;

        virtual WinningState score(int player) const = 0;
    };
}