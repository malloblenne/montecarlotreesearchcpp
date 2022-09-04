#include "montecarlotreesearch.h"
#include <thread>
#include <chrono>
 
using namespace mctsearch;
using namespace std::chrono_literals;


MonteCarloTreeSearch::MonteCarloTreeSearch(MonteCarloTreeSearch::MonteCarloTreeSearchInitialization init):
                                           selection{std::move(init.selection)},
                                           backup{std::move(init.backup)},
                                           simulation{std::move(init.simulation)},
                                           resourcecriteria{std::move(init.resourcecriteria)},
                                           bestchild{std::move(init.bestchild)},
                                           statistics{std::move(init.statistics)}
                                           {

                                           }


void MonteCarloTreeSearch::init()
{
    resourcecriteria->init();
}

std::unique_ptr<Action> MonteCarloTreeSearch::search(const GameState& state)
{
    root = std::make_shared<TreeSearchNode>(state.clone(), statistics->clone(), nullptr);
    auto new_node{root.get()};

    while(!resourcecriteria->expired())
    {
        // just for quickly test
        std::this_thread::sleep_for(2000ms);
        new_node = selection->select_new(new_node);
        const auto reward = simulation->simulate(new_node->game_state());
        backup->update(new_node, reward);
    }

    return bestchild->choose_action(new_node);
}