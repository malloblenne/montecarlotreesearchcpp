#include <../src/montecarlotreesearchfactory.h>
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
        mcts.set_diagnostics_enabled(true);
        std::cout /*<< "\033[2J\033[H" */<< game.to_string() << std::flush;
        auto action = mcts.search(game);
        std::cout << "Action will be: " << action->to_string() << std::endl;
        game.apply_nth_move(action->value());
    }
    std::cout /*<< "\033[2J\033[H" */<< game.to_string() << std::flush;
}

int main()
{
    auto mcts{MonteCarloTreeSearchFactory::make_SimpleMonteCarloTreeSearch()};

    TicTacToe game(std::array<std::array<int, 3>, 3>{{{0,2,1},{2,2,0},{1,1,0}}}, TicTacToe::PLAYER_X);
    play_game(game, *mcts, "Tic-Tac-Toe");

    //Hex hex;
    //play_game(hex, *mcts, "Hex");
    return 0;
}