#pragma once

#include "../src/gamestate.h"
#include <array>


namespace mctsearch
{
    class TicTacToe: public GameState
    {
        public:

        static const int PLAYER_NOPLAYER = 0;
        static const int PLAYER_X = 1;
        static const int PLAYER_O = 2;

        TicTacToe() = default;

        TicTacToe(std::array<std::array<int, 3>, 3> board, int owner);

        TicTacToe(const TicTacToe& origin);

        TicTacToe &operator=(TicTacToe const &origin);

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
        int which_player_wins_by_row() const;
        int which_player_wins_by_columns() const;
        int which_player_wins_by_diagonals() const;
        bool is_board_full() const;

        std::array<std::array<int, 3>, 3> board{{ {0,0,0}, {0,0,0}, {0,0,0}}};
        int owner = PLAYER_X;
       
    };
}