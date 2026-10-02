#include "tictactoe.h"
#include "../src/action.h"

#include <string>
#include <algorithm>
#include <iostream>


using namespace mctsearch;
using namespace std;

TicTacToe::TicTacToe(std::array<std::array<int, 3>, 3> board_p, int owner_p):
board(board_p),
owner(owner_p)
{
}


TicTacToe::TicTacToe(const TicTacToe& origin):
board{origin.board},
owner{origin.owner}
{

}

TicTacToe& TicTacToe::operator=(TicTacToe const &origin)
{
    board = origin.board;
    owner = origin.owner;
    return *this;  
}

std::string TicTacToe::to_string() const
{
    std::string result;
    for (int r = 0; r < board.size(); r++)
    {
        for (int c = 0; c <  board[0].size(); c++)
        {
            result += std::to_string(board[r][c]) + " ";
        }
        result += '\n';
    }
    return result;
}

int TicTacToe::number_moves()
{
    //it would be nice to use ranges in c++20 to flatten and create a view and get the numbers
    int number_zeros{0};
    for (int r = 0; r < board.size(); r++)
    {
        for (int c = 0; c < board[0].size(); c++)
        {
            if (board[r][c] == 0)
            {
                number_zeros++;
            }
        }
    }
    return number_zeros;
}

void TicTacToe::apply_nth_move(int n)
{
    // it assumes preconditions are fine and n is valid

    //board[n /  board.size()][ n % board[0].size()] = get_owner();
    int counter = 0;
    for (int r = 0; r < board.size(); r++)
    {
        for (int c = 0; c < board[0].size(); c++)
        {
            if (board[r][c] == PLAYER_NOPLAYER)
            {
                if (counter == n)
                {
                    board[r][c] = get_owner();
                    owner = owner == PLAYER_X ? PLAYER_O : PLAYER_X;
                    return;
                }
                counter++;
            }
        }
    }
    throw std::invalid_argument(std::string("Invalid Argument. Max available n is ") + std::to_string(counter - 1));
}

int TicTacToe::get_owner() const
{
    return owner;
}

bool TicTacToe::is_terminal() const
{
    return which_player_wins_by_row() != PLAYER_NOPLAYER  ||
           which_player_wins_by_columns() != PLAYER_NOPLAYER ||
           which_player_wins_by_diagonals() != PLAYER_NOPLAYER ||
           is_board_full();
}

int TicTacToe::which_player_wins_by_row() const
{
    for (int r = 0; r < board.size(); r++)
    {
         // assume more than 1x1
         if (std::all_of(board[r].begin()+1, board[r].end(), [&] (int i) {return i == board[r][0];}))
         {
            return board[r][0];
         }
    }
    return PLAYER_NOPLAYER;
}
int TicTacToe::which_player_wins_by_columns() const
{
    // assume square
    for (int c = 0; c < board[0].size(); c++)
    {
        bool same = true;
        const int reference = board[0][c];
        for (int r = 1; r < board.size() && same; r++)
        {
            same = board[r][c] == reference;
        }
        if (same)
        {
            return reference;
        }
    }
    return PLAYER_NOPLAYER;
}
int TicTacToe::which_player_wins_by_diagonals() const
{
    // first diagonal
    int reference = board[0][0];
    bool same = true;
    for (int r = 1; r < board.size() && same; r++)
    {
        same = reference == board[r][r];
    }
    if (same)
    {
        return reference;
    }
    // opposite diagonal
    reference = board[0][board.size()-1];
    same = true;
    for (int r = 1; r < board.size() && same; r++)
    {
        same = reference == board[r][board.size()-r-1];
    }
    if (same)
    {
        return reference;
    }
    return PLAYER_NOPLAYER;
}

bool TicTacToe::is_board_full() const
{
    for (int r = 0; r < board.size(); r++)
    {
        for (int c = 0; c < board[0].size(); c++)
        {
            if (board[r][c] == PLAYER_NOPLAYER)
            {
                return false;
            }
        }
    }
    return true;
}

std::vector<std::string> TicTacToe::drawables() const
{
    return std::vector<std::string>();
}

WinningState TicTacToe::score(int player_requested) const
{
    int player = which_player_wins_by_row();
    if (player != PLAYER_NOPLAYER)
    {
        return player_requested == player ? WinningState::WIN : WinningState::LOSS;
    }
    player = which_player_wins_by_columns();
    if (player != PLAYER_NOPLAYER)
    {
        return player_requested == player ? WinningState::WIN : WinningState::LOSS;
    }
    player = which_player_wins_by_diagonals();
    if (player != PLAYER_NOPLAYER)
    {
        return player_requested == player ? WinningState::WIN : WinningState::LOSS;
    }
    return WinningState::DRAW;
}

std::unique_ptr<GameState> TicTacToe::clone() const
{
    return std::move(std::make_unique<TicTacToe>(*this));
}


class TicTacToeAction: public Action
{
    public:
        TicTacToeAction(int value, int row, int column):
        v(value),
        row(row),
        column(column)
        {

        }

        TicTacToeAction() = delete;

    std::string to_string() const
    {
        return std::string("(")+ std::to_string(row) +
               std::string(", ") + std::to_string(column) +
               std::string(")")+ std::string(" n: ")  + std::to_string(v);
    }
    int value() const
    {
        return v;
    }

    private:
        int v;
        int row;
        int column;
};

std::unique_ptr<Action> TicTacToe::get_action_nth(int n) const
{
    //find the nth available action from top-left to bottom-right
    int counter = 0;
    for (int r = 0; r < board.size(); r++)
    {
        for (int c = 0; c < board[0].size(); c++)
        {
            if (board[r][c] == PLAYER_NOPLAYER)
            {
                if (counter == n)
                {
                    return std::make_unique<TicTacToeAction>(n, r, c);
                }
                counter++;
            }
        }
    }

       throw std::invalid_argument(std::string("Invalid Argument. Max available n is ") + std::to_string(counter - 1));
}