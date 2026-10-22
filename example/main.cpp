#include "montecarlotreesearchfactory.h"
#include "circleoflife.h"
#include "hex.h"
#include "tictactoe.h"
#include <iostream>

using namespace mctsearch;

void play_game(GameState& game, MonteCarloTreeSearch& mcts, const std::string& name)
{
    std::cout << name << std::endl;
    while (!game.is_terminal())
    {
        mcts.init();
        //mcts.set_diagnostics_enabled(true);
        std::cout << /*"\033[2J\033[H" <<*/ game.to_string() << std::flush;
        auto action = mcts.search(game);
        std::cout << "Action will be: " << action->to_string() << std::endl;
        game.apply_nth_move(action->value());
    }
    std::cout << /*"\033[2J\033[H" <<*/ game.to_string() << std::flush;
}

int main()
{
    TicTacToe game(std::array<std::array<int, 3>, 3>{{{0,2,1},{2,2,0},{1,1,0}}}, TicTacToe::PLAYER_X);
    auto tic_tac_toe_mcts{MonteCarloTreeSearchFactory::make_SimpleMonteCarloTreeSearch(1000)};
    //play_game(game, *tic_tac_toe_mcts, "Tic-Tac-Toe");

    Hex hex;
    auto hex_mcts{MonteCarloTreeSearchFactory::make_SimpleMonteCarloTreeSearch(1000)};
    //play_game(hex, *hex_mcts, "Hex");

    CircleOfLife circle_of_life;
    auto circle_of_life_mcts{MonteCarloTreeSearchFactory::make_SimpleMonteCarloTreeSearch(100)};
    play_game(circle_of_life, *circle_of_life_mcts, "Circle of Life");
    return 0;
}