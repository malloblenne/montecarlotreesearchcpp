#include "../inc/montecarlotreesearch.h"
#include <thread>
 
using namespace MonteCarloTreeSearch;
using namespace std::chrono_literals;

MonteCarloTreeSearch::MonteCarloTreeSearch(MonteCarloTreeSearch::MonteCarloTreeSearch::MonteCarloTreeSearchInitialization init):
                                           selection(std::move(init.selection)),
                                           expansion(std::move(init.expansion)),
                                           backup(std::move(init.backup)),
                                           simulation(std::move(init.simulation)),
                                           duration(init.duration)
                                           {

                                           }


void MonteCarloTreeSearch::init(std::shared_ptr<GameState> state)
{

}

std::unique_ptr<Action> MonteCarloTreeSearch::search(std::shared_ptr<GameState> state)
{
    const auto start = chrono::steady_clock::now();
    const auto still_time = [start, this]() { return (chrono::steady_clock::now() - start) < duration;};
    while(still_time)
    {
        std::this_thread::sleep_for(2000ms);
    }
    return std::unique_ptr<Action>;
}