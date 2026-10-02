#include "circleoflife.h"
#include "../src/action.h"

#include <algorithm>
#include <array>
#include <iomanip>
#include <map>
#include <queue>
#include <sstream>
#include <stdexcept>

using namespace mctsearch;

const std::array<CircleOfLife::Cell, 6> CircleOfLife::DIRECTIONS{{
    {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}
}};

const std::array<std::string, CircleOfLife::SPECIES_COUNT> CircleOfLife::SPECIES_KEYS{{
    "0,0",
    "0,0|0,1",
    "0,0|0,1|1,1",
    "0,0|0,1|0,2",
    "0,0|0,1|1,0",
    "0,0|0,1|0,2|1,0",
    "0,0|0,1|0,2|1,2",
    "0,0|0,1|1,1|2,0",
    "0,0|0,1|1,1|1,2",
    "0,0|0,1|1,0|1,1",
    "0,1|1,1|1,2|2,0",
    "0,0|0,1|0,2|0,3"
}};

CircleOfLife::CircleOfLife() = default;

CircleOfLife::CircleOfLife(const CircleOfLife& origin):
board{origin.board},
owner{origin.owner},
captures{origin.captures},
winner{origin.winner}
{
}

CircleOfLife& CircleOfLife::operator=(CircleOfLife const& origin)
{
    board = origin.board;
    owner = origin.owner;
    captures = origin.captures;
    winner = origin.winner;
    return *this;
}

std::string CircleOfLife::to_string() const
{
    std::ostringstream result;
    for (int r = -BOARD_RADIUS; r <= BOARD_RADIUS; r++)
    {
        const int first_q = std::max(-BOARD_RADIUS, -r - BOARD_RADIUS);
        const int last_q = std::min(BOARD_RADIUS, -r + BOARD_RADIUS);
        result << std::string(static_cast<std::size_t>(std::abs(r)), ' ');
        for (int q = first_q; q <= last_q; q++)
        {
            result << at(q, r);
            if (q < last_q)
            {
                result << " - ";
            }
        }
        result << '\n';
    }
    result << "captures: " << captures[0] << "-" << captures[1] << '\n';
    return result.str();
}

int CircleOfLife::number_moves()
{
    return static_cast<int>(legal_moves().size());
}

void CircleOfLife::apply_nth_move(int n)
{
    const auto moves = legal_moves();
    if (n < 0 || n >= static_cast<int>(moves.size()))
    {
        throw std::invalid_argument("Invalid Circle of Life move index: " + std::to_string(n));
    }
    place(moves[n]);
}

int CircleOfLife::get_owner() const
{
    return owner;
}

bool CircleOfLife::is_terminal() const
{
    return winner != PLAYER_NOPLAYER || board_full() || legal_moves().empty();
}

std::vector<std::string> CircleOfLife::drawables() const
{
    return {};
}

WinningState CircleOfLife::score(int player) const
{
    if (winner != PLAYER_NOPLAYER)
    {
        return winner == player ? WinningState::WIN : WinningState::LOSS;
    }
    return WinningState::DRAW;
}

std::unique_ptr<GameState> CircleOfLife::clone() const
{
    return std::make_unique<CircleOfLife>(*this);
}

class CircleOfLifeAction: public Action
{
    public:
    CircleOfLifeAction(int value, int q, int r):
    value_{value},
    q_{q},
    r_{r}
    {
    }

    std::string to_string() const override
    {
        return "(" + std::to_string(q_) + ", " + std::to_string(r_) + ") n: " +
               std::to_string(value_);
    }

    int value() const override
    {
        return value_;
    }

    private:
    int value_;
    int q_;
    int r_;
};

std::unique_ptr<Action> CircleOfLife::get_action_nth(int n) const
{
    const auto moves = legal_moves();
    if (n < 0 || n >= static_cast<int>(moves.size()))
    {
        throw std::invalid_argument("Invalid Circle of Life action index: " + std::to_string(n));
    }
    return std::make_unique<CircleOfLifeAction>(n, moves[n].first, moves[n].second);
}

bool CircleOfLife::valid(int q, int r) const
{
    return std::max({std::abs(q), std::abs(r), std::abs(q + r)}) <= BOARD_RADIUS;
}

int& CircleOfLife::at(int q, int r)
{
    return board[r + BOARD_RADIUS][q + BOARD_RADIUS];
}

int CircleOfLife::at(int q, int r) const
{
    return board[r + BOARD_RADIUS][q + BOARD_RADIUS];
}

std::vector<CircleOfLife::Cell> CircleOfLife::cells() const
{
    std::vector<Cell> result;
    for (int r = -BOARD_RADIUS; r <= BOARD_RADIUS; r++)
    {
        for (int q = -BOARD_RADIUS; q <= BOARD_RADIUS; q++)
        {
            if (valid(q, r))
            {
                result.emplace_back(q, r);
            }
        }
    }
    return result;
}

std::vector<CircleOfLife::Cell> CircleOfLife::neighbours(const Cell& cell) const
{
    std::vector<Cell> result;
    for (const auto [dq, dr] : DIRECTIONS)
    {
        const Cell next{cell.first + dq, cell.second + dr};
        if (valid(next.first, next.second))
        {
            result.push_back(next);
        }
    }
    return result;
}

std::vector<CircleOfLife::Group> CircleOfLife::groups(int player) const
{
    std::vector<Group> result;
    std::vector<Cell> pieces;
    for (const auto cell : cells())
    {
        if (at(cell.first, cell.second) == player)
        {
            pieces.push_back(cell);
        }
    }

    std::vector<Cell> todo;
    for (const auto& piece : pieces)
    {
        bool seen = false;
        for (const auto& group : result)
        {
            if (std::find(group.begin(), group.end(), piece) != group.end())
            {
                seen = true;
                break;
            }
        }
        if (seen)
        {
            continue;
        }

        Group group;
        todo = {piece};
        while (!todo.empty())
        {
            const Cell current = todo.back();
            todo.pop_back();
            if (std::find(group.begin(), group.end(), current) != group.end())
            {
                continue;
            }
            group.push_back(current);
            for (const auto& next : neighbours(current))
            {
                if (at(next.first, next.second) == player &&
                    std::find(group.begin(), group.end(), next) == group.end())
                {
                    todo.push_back(next);
                }
            }
        }
        result.push_back(std::move(group));
    }
    return result;
}

bool CircleOfLife::adjacent(const Cell& cell, const Group& group) const
{
    for (const auto& next : neighbours(cell))
    {
        if (std::find(group.begin(), group.end(), next) != group.end())
        {
            return true;
        }
    }
    return false;
}

bool CircleOfLife::adjacent(const Group& first, const Group& second) const
{
    for (const auto& cell : first)
    {
        if (adjacent(cell, second))
        {
            return true;
        }
    }
    return false;
}

CircleOfLife::Group CircleOfLife::new_group(const Cell& cell, int player) const
{
    Group result{cell};
    for (const auto& group : groups(player))
    {
        if (adjacent(cell, group))
        {
            result.insert(result.end(), group.begin(), group.end());
        }
    }
    return result;
}

std::vector<CircleOfLife::Cell> CircleOfLife::legal_moves() const
{
    std::vector<Cell> result;
    for (const auto& cell : cells())
    {
        if (at(cell.first, cell.second) == PLAYER_NOPLAYER &&
            new_group(cell, owner).size() <= 4)
        {
            result.push_back(cell);
        }
    }
    return result;
}

std::string CircleOfLife::canonical_key(const Group& group, const Cell& root) const
{
    std::array<std::array<int, 3>, 4> base{};
    for (std::size_t i = 0; i < group.size(); i++)
    {
        const int q = group[i].first - root.first;
        const int r = group[i].second - root.second;
        base[i] = {q, r, -q - r};
    }

    std::vector<std::string> keys;
    for (int flip = 0; flip < 2; flip++)
    {
        for (int rotation = 0; rotation < 6; rotation++)
        {
            std::vector<Cell> transformed;
            for (std::size_t i = 0; i < group.size(); i++)
            {
                int q = base[i][0];
                int r = base[i][1];
                int s = base[i][2];
                if (flip != 0)
                {
                    std::tie(q, r, s) = std::make_tuple(s, r, q);
                }
                for (int step = 0; step < rotation; step++)
                {
                    std::tie(q, r, s) = std::make_tuple(-r, -s, -q);
                }
                transformed.emplace_back(q, r);
            }

            int min_q = transformed.front().first;
            int min_r = transformed.front().second;
            for (const auto [q, r] : transformed)
            {
                min_q = std::min(min_q, q);
                min_r = std::min(min_r, r);
            }
            std::vector<std::string> normalized;
            for (const auto [q, r] : transformed)
            {
                normalized.push_back(std::to_string(q - min_q) + "," +
                                     std::to_string(r - min_r));
            }
            std::sort(normalized.begin(), normalized.end());
            std::ostringstream key;
            for (std::size_t i = 0; i < normalized.size(); i++)
            {
                if (i != 0)
                {
                    key << '|';
                }
                key << normalized[i];
            }
            keys.push_back(key.str());
        }
    }
    return *std::min_element(keys.begin(), keys.end());
}

int CircleOfLife::species_index(const Group& group) const
{
    std::vector<std::string> keys;
    for (const auto& root : group)
    {
        keys.push_back(canonical_key(group, root));
    }
    return static_cast<int>(std::find(SPECIES_KEYS.begin(), SPECIES_KEYS.end(),
                                      *std::min_element(keys.begin(), keys.end())) -
                            SPECIES_KEYS.begin());
}

bool CircleOfLife::has_winner(int player) const
{
    return winner == player;
}

bool CircleOfLife::board_full() const
{
    for (const auto& cell : cells())
    {
        if (at(cell.first, cell.second) == PLAYER_NOPLAYER)
        {
            return false;
        }
    }
    return true;
}

void CircleOfLife::place(const Cell& cell)
{
    const int mover = owner;
    const Group predator = new_group(cell, mover);
    at(cell.first, cell.second) = mover;
    const int species = species_index(predator);
    const int prey = (species + SPECIES_COUNT - 1) % SPECIES_COUNT;

    for (const auto& group : groups(mover == PLAYER_X ? PLAYER_O : PLAYER_X))
    {
        if (species_index(group) == prey && adjacent(predator, group))
        {
            for (const auto& captured : group)
            {
                at(captured.first, captured.second) = PLAYER_NOPLAYER;
            }
            captures[mover - 1] += static_cast<int>(group.size());
        }
    }

    if (captures[mover - 1] >= CAPTURE_THRESHOLD)
    {
        winner = mover;
        return;
    }

    owner = mover == PLAYER_X ? PLAYER_O : PLAYER_X;
    if (legal_moves().empty())
    {
        winner = owner;
    }
}
