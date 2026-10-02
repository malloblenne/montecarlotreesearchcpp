#pragma once

#include "gamestate.h"

#include <array>
#include <utility>
#include <vector>

namespace mctsearch
{
    class CircleOfLife: public GameState
    {
        public:
        static constexpr int BOARD_RADIUS = 4;
        static constexpr int BOARD_SIZE = BOARD_RADIUS * 2 + 1;
        static constexpr int CAPTURE_THRESHOLD = 20;
        static constexpr int SPECIES_COUNT = 12;

        static constexpr int PLAYER_NOPLAYER = 0;
        static constexpr int PLAYER_X = 1;
        static constexpr int PLAYER_O = 2;

        CircleOfLife();
        CircleOfLife(const CircleOfLife& origin);
        CircleOfLife& operator=(CircleOfLife const& origin);

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
        using Cell = std::pair<int, int>;
        using Group = std::vector<Cell>;

        static const std::array<Cell, 6> DIRECTIONS;
        static const std::array<std::string, SPECIES_COUNT> SPECIES_KEYS;

        bool valid(int q, int r) const;
        int& at(int q, int r);
        int at(int q, int r) const;
        std::vector<Cell> cells() const;
        std::vector<Cell> neighbours(const Cell& cell) const;
        std::vector<Group> groups(int player) const;
        bool adjacent(const Cell& cell, const Group& group) const;
        bool adjacent(const Group& first, const Group& second) const;
        Group new_group(const Cell& cell, int player) const;
        std::vector<Cell> legal_moves() const;
        int species_index(const Group& group) const;
        std::string canonical_key(const Group& group, const Cell& root) const;
        bool has_winner(int player) const;
        bool board_full() const;
        void place(const Cell& cell);

        std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> board{};
        int owner = PLAYER_X;
        std::array<int, 2> captures{};
        int winner = PLAYER_NOPLAYER;
    };
}
