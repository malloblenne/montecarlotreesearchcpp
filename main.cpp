#include "circleoflife.h"
#include "hex.h"
#include "montecarlotreesearchfactory.h"
#include "tictactoe.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
bool quit_requested(const std::string& line)
{
    std::istringstream input(line);
    std::string command;
    std::string extra;
    return (input >> command) && (command == "q" || command == "Q") && !(input >> extra);
}

template<typename Game>
void play_game(Game game, const std::string& name, const std::string& format, int iterations)
{
    const int human = Game::PLAYER_X;
    auto ai = mctsearch::MonteCarloTreeSearchFactory::make_SimpleMonteCarloTreeSearch(iterations);
    std::cout << '\n' << name << "\nYou: X (1), AI: O (2), empty: 0. You move first.\n"
              << format << "\nEnter q to quit.\n\n" << game.to_string();

    while (!game.is_terminal())
    {
        const int count = game.number_moves();
        if (count == 0)
        {
            throw std::runtime_error("Non-terminal game has no legal actions.");
        }
        if (game.get_owner() == human)
        {
            std::cout << "Legal actions: ";
            for (int index = 0; index < std::min(10, count); ++index)
            {
                if (index != 0)
                {
                    std::cout << "; ";
                }
                std::cout << game.get_action_nth(index)->to_string();
            }
            std::cout << '\n';
            if (count > 10)
            {
                std::cout << count - 10 << " more actions available.\n";
            }
            while (true)
            {
                std::cout << "Your move: " << std::flush;
                std::string line;
                if (!std::getline(std::cin, line) || quit_requested(line))
                {
                    std::cout << "\nGoodbye.\n";
                    return;
                }
                std::istringstream input(line);
                int first = 0;
                int second = 0;
                std::string extra;
                if (!(input >> first >> second) || (input >> extra))
                {
                    std::cout << "Invalid input: enter exactly two integer coordinates.\n";
                    continue;
                }
                const auto index = game.move_index(first, second);
                if (!index)
                {
                    std::cout << "Illegal move: choose an available coordinate.\n";
                    continue;
                }
                game.apply_nth_move(*index);
                break;
            }
        }
        else
        {
            std::cout << "AI thinking...\n" << std::flush;
            ai->init();
            const auto action = ai->search(game);
            if (!action || action->value() < 0 || action->value() >= count)
            {
                throw std::runtime_error("AI returned an invalid action.");
            }
            std::cout << "AI move: " << action->to_string() << '\n';
            game.apply_nth_move(action->value());
        }
        std::cout << '\n' << game.to_string();
    }

    switch (game.score(human))
    {
        case mctsearch::WinningState::WIN: std::cout << "You win!\n"; break;
        case mctsearch::WinningState::LOSS: std::cout << "AI wins.\n"; break;
        case mctsearch::WinningState::DRAW: std::cout << "Draw.\n"; break;
    }
}
}

int main()
{
    try
    {
        std::cout << "Select a game:\n1. TicTacToe\n2. Hex\n3. Circle of Life\nq. Quit\n";
        while (true)
        {
            std::cout << "Game: " << std::flush;
            std::string line;
            if (!std::getline(std::cin, line) || quit_requested(line))
            {
                std::cout << "\nGoodbye.\n";
                return 0;
            }
            std::istringstream input(line);
            int choice = 0;
            std::string extra;
            if (!(input >> choice) || (input >> extra) || choice < 1 || choice > 3)
            {
                std::cout << "Choose 1, 2, 3, or q.\n";
                continue;
            }
            if (choice == 1)
            {
                play_game(mctsearch::TicTacToe{}, "TicTacToe",
                          "Input: row col, both 0..2 (example: 1 2). Rows increase downward; columns rightward.", 1000);
            }
            else if (choice == 2)
            {
                play_game(mctsearch::Hex{}, "Hex",
                          "Input: row col, both 0..10 (example: 3 5). Rows increase downward; columns rightward.\n"
                          "X connects top to bottom; O connects left to right.", 1000);
            }
            else
            {
                play_game(mctsearch::CircleOfLife{}, "Circle of Life",
                          "Input: q r, both -4..4, with q+r also -4..4 (example: -1 2).\n"
                          "Rows are labeled r; q increases left to right over each row's labeled range.", 100);
            }
            return 0;
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}