#include "catch2/catch.hpp"
#include "circleoflife.h"
#include "hex.h"
#include "tictactoe.h"

#include <limits>

using namespace mctsearch;

template<typename Game>
void check_coordinates(Game& game, int minimum, int maximum, bool axial)
{
    int index = 0;
    for (int row = minimum; row <= maximum; ++row)
    {
        for (int column = minimum; column <= maximum; ++column)
        {
            const int first = axial ? column : row;
            const int second = axial ? row : column;
            const auto move = game.move_index(first, second);
            if (axial && (first + second < minimum || first + second > maximum))
            {
                REQUIRE_FALSE(move.has_value());
                continue;
            }
            REQUIRE(move == index);
            const auto action = game.get_action_nth(index);
            REQUIRE(action->to_string() == "(" + std::to_string(first) + ", " +
                    std::to_string(second) + ") n: " + std::to_string(index));
            Game copy(game);
            copy.apply_nth_move(*move);
            REQUIRE_FALSE(copy.move_index(first, second).has_value());
            REQUIRE(copy.get_owner() == static_cast<int>(Game::PLAYER_O));
            REQUIRE(game.get_owner() == static_cast<int>(Game::PLAYER_X));
            ++index;
        }
    }
    REQUIRE(game.number_moves() == index);
    REQUIRE_FALSE(game.move_index(minimum - 1, 0).has_value());
    REQUIRE_FALSE(game.move_index(0, maximum + 1).has_value());
    REQUIRE_FALSE(game.move_index(std::numeric_limits<int>::min(), 0).has_value());
    REQUIRE_FALSE(game.move_index(0, std::numeric_limits<int>::max()).has_value());
    game.apply_nth_move(0);
    REQUIRE(game.move_index(axial ? minimum + 1 : 0, axial ? 0 : 1).has_value());
}

TEST_CASE("TicTacToe coordinates follow current legal action ordering", "[games]")
{
    TicTacToe game;
    check_coordinates(game, 0, 2, false);
    REQUIRE(game.move_index(0, 1) == 0);
    REQUIRE(game.move_index(1, 0) == 2);
}

TEST_CASE("Hex coordinates follow current legal action ordering", "[games]")
{
    Hex game;
    check_coordinates(game, 0, Hex::BOARD_SIZE - 1, false);
    REQUIRE(game.move_index(0, 1) == 0);
    REQUIRE(game.move_index(10, 10) == 119);
}

TEST_CASE("Circle of Life coordinates include negative axial positions", "[games]")
{
    CircleOfLife game;
    check_coordinates(game, -CircleOfLife::BOARD_RADIUS, CircleOfLife::BOARD_RADIUS, true);
    REQUIRE_FALSE(game.move_index(4, 4).has_value());
    REQUIRE_FALSE(game.move_index(-4, -4).has_value());
    REQUIRE(game.move_index(1, -4) == 0);
}

TEST_CASE("Circle of Life coordinate lookup rejects groups larger than four", "[games]")
{
    CircleOfLife game;
    for (int column = 0; column < 4; ++column)
    {
        const auto human = game.move_index(column, 0);
        REQUIRE(human.has_value());
        game.apply_nth_move(*human);
        const auto opponent = game.move_index(column, -4);
        REQUIRE(opponent.has_value());
        game.apply_nth_move(*opponent);
    }
    REQUIRE_FALSE(game.is_terminal());
    REQUIRE(game.get_owner() == CircleOfLife::PLAYER_X);
    REQUIRE_FALSE(game.move_index(4, 0).has_value());
    REQUIRE(game.move_index(-4, 4).has_value());
}