#include "hex.h"
#include "../src/action.h"

#include <algorithm>
#include <queue>
#include <sstream>
#include <stdexcept>

using namespace mctsearch;

Hex::Hex() = default;

Hex::Hex(const Hex& origin):
board{origin.board},
owner{origin.owner}
{
}

Hex& Hex::operator=(Hex const& origin)
{
    board = origin.board;
    owner = origin.owner;
    return *this;
}

std::string Hex::to_string() const
{
    std::ostringstream result;
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        result << std::string(static_cast<std::size_t>(row), ' ');
        for (int column = 0; column < BOARD_SIZE; column++)
        {
            result << board[row][column];
            if (column + 1 < BOARD_SIZE)
            {
                result << " - ";
            }
        }
        result << '\n';
        if (row + 1 < BOARD_SIZE)
        {
            result << std::string(static_cast<std::size_t>(row + 1), ' ');
            for (int column = 0; column < BOARD_SIZE; column++)
            {
                result << "\\ /";
                if (column + 1 < BOARD_SIZE)
                {
                    result << ' ';
                }
            }
            result << '\n';
        }
    }
    return result.str();
}

int Hex::number_moves()
{
    int available = 0;
    for (const auto& row : board)
    {
        available += static_cast<int>(std::count(row.begin(), row.end(), PLAYER_NOPLAYER));
    }
    return available;
}

void Hex::apply_nth_move(int n)
{
    int counter = 0;
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int column = 0; column < BOARD_SIZE; column++)
        {
            if (board[row][column] == PLAYER_NOPLAYER)
            {
                if (counter == n)
                {
                    board[row][column] = owner;
                    owner = owner == PLAYER_X ? PLAYER_O : PLAYER_X;
                    return;
                }
                counter++;
            }
        }
    }
    throw std::invalid_argument("Invalid Hex move index: " + std::to_string(n));
}

int Hex::get_owner() const
{
    return owner;
}

bool Hex::is_terminal() const
{
    return has_connection(PLAYER_X) || has_connection(PLAYER_O) || is_board_full();
}

std::vector<std::string> Hex::drawables() const
{
    return {};
}

WinningState Hex::score(int player) const
{
    if (has_connection(player))
    {
        return WinningState::WIN;
    }
    if (has_connection(player == PLAYER_X ? PLAYER_O : PLAYER_X))
    {
        return WinningState::LOSS;
    }
    return WinningState::DRAW;
}

std::unique_ptr<GameState> Hex::clone() const
{
    return std::make_unique<Hex>(*this);
}

class HexAction: public Action
{
    public:
    HexAction(int value, int row, int column):
    value_{value},
    row_{row},
    column_{column}
    {
    }

    std::string to_string() const override
    {
        return "(" + std::to_string(row_) + ", " + std::to_string(column_) + ") n: " +
               std::to_string(value_);
    }

    int value() const override
    {
        return value_;
    }

    private:
    int value_;
    int row_;
    int column_;
};

std::unique_ptr<Action> Hex::get_action_nth(int n) const
{
    int counter = 0;
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int column = 0; column < BOARD_SIZE; column++)
        {
            if (board[row][column] == PLAYER_NOPLAYER)
            {
                if (counter == n)
                {
                    return std::make_unique<HexAction>(n, row, column);
                }
                counter++;
            }
        }
    }
    throw std::invalid_argument("Invalid Hex action index: " + std::to_string(n));
}

bool Hex::has_connection(int player) const
{
    std::queue<std::pair<int, int>> pending;
    std::array<std::array<bool, BOARD_SIZE>, BOARD_SIZE> visited{};

    if (player == PLAYER_X)
    {
        for (int column = 0; column < BOARD_SIZE; column++)
        {
            if (board[0][column] == player)
            {
                pending.emplace(0, column);
                visited[0][column] = true;
            }
        }
    }
    else
    {
        for (int row = 0; row < BOARD_SIZE; row++)
        {
            if (board[row][0] == player)
            {
                pending.emplace(row, 0);
                visited[row][0] = true;
            }
        }
    }

    constexpr std::array<std::pair<int, int>, 6> directions{{
        {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}
    }};

    while (!pending.empty())
    {
        const auto [row, column] = pending.front();
        pending.pop();

        if ((player == PLAYER_X && row == BOARD_SIZE - 1) ||
            (player == PLAYER_O && column == BOARD_SIZE - 1))
        {
            return true;
        }

        for (const auto [row_delta, column_delta] : directions)
        {
            const int next_row = row + row_delta;
            const int next_column = column + column_delta;
            if (next_row >= 0 && next_row < BOARD_SIZE &&
                next_column >= 0 && next_column < BOARD_SIZE &&
                !visited[next_row][next_column] &&
                board[next_row][next_column] == player)
            {
                visited[next_row][next_column] = true;
                pending.emplace(next_row, next_column);
            }
        }
    }
    return false;
}

bool Hex::is_board_full() const
{
    for (const auto& row : board)
    {
        if (std::count(row.begin(), row.end(), PLAYER_NOPLAYER) != 0)
        {
            return false;
        }
    }
    return true;
}
