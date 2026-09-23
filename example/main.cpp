#include <../src/montecarlotreesearchfactory.h>
#include "tictactoe.h"
#include <iostream>

using namespace mctsearch;

int main()
{
    auto mcts{MonteCarloTreeSearchFactory::make_SimpleMonteCarloTreeSearch()};

    //TicTacToe game;

    TicTacToe game(std::array<std::array<int, 3>, 3>{{{0,2,1},{2,2,0},{1,1,0}}}, TicTacToe::PLAYER_X);
    //Initial state
    std::cout << game.to_string() << std::endl;

    // 0 2 1 
    // 2 2 0 
    // 1 1 0 

    while (!game.is_terminal())
    {
        // Turn
        mcts->init();
        std::cout << game.to_string() << std::endl;
        auto action = mcts->search(game);        
        std::cout << "Action will be: " << action->to_string() << std::endl;
        game.apply_nth_move(action->value());
    }
    std::cout << game.to_string() << std::endl;

    
    return 0;
}