#include "../inc/montecarlotreesearch.h"
#include <thread>
 
using namespace MonteCarloTreeSearch;

MonteCarloTreeSearch::MonteCarloTreeSearch(MonteCarloTreeSearch::MonteCarloTreeSearch::MonteCarloTreeSearchInitialization init):
                                           selection(std::move(init.selection)),
                                           expansion(std::move(init.expansion)),
                                           backup(std::move(init.backup)),
                                           simulation(std::move(init.simulation)),
                                           resoucecriteria(std::move(init.resourcecriteria))
                                           {

                                           }


void MonteCarloTreeSearch::init(std::shared_ptr<GameState> state)
{
    resourcecriteria->init();
}

std::unique_ptr<Action> MonteCarloTreeSearch::search(std::shared_ptr<GameState> state)
{

    while(!resourcecriteria->expired())
    {
        std::this_thread::sleep_for(2000ms);
    }
    return std::unique_ptr<Action>;
}