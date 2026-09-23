#pragma once

#include "../src/gamestate.h"
#include <array>

namespace mctsearch
{
    class Hex: public GameState
    {
        public:
        static constexpr int BOARD_SIZE = 11;
        static constexpr int PLAYER_NOPLAYER = 0;
        static constexpr int PLAYER_X = 1;
        static constexpr int PLAYER_O = 2;

        Hex();
        Hex(const Hex& origin);
        Hex& operator=(Hex const& origin);

        std::string to_string() const;
        int number_moves();
        void apply_nth_move(int n);
        int get_owner() const;
        bool is_terminal() const;
        std::vector<std::string> drawables() const;
        WinningState score(int player) const;
        std::unique_ptr<GameState> clone() const;
        std::unique_ptr<Action> get_action_nth(int n) const;

        private:
        bool has_connection(int player) const;
        bool is_board_full() const;

        std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> board{};
        int owner = PLAYER_X;
    };
}
