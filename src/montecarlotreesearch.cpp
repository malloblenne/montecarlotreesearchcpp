#include "montecarlotreesearch.h"

#include <iostream>

using namespace mctsearch;


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

void MonteCarloTreeSearch::set_diagnostics_enabled(bool enabled)
{
    diagnostics_enabled = enabled;
}

void MonteCarloTreeSearch::log_tree(const TreeSearchNode* node, int depth) const
{
    const auto* statisticsnode = node->statistics();
    std::clog << std::string(static_cast<std::size_t>(depth) * 2, ' ')
              << "node visits=" << statisticsnode->visit_count()
              << " reward=" << statisticsnode->simulation_reward() << '\n';

    for (const auto& explored : node->children_explored())
    {
        const auto& action = explored.first;
        const auto& child = explored.second;
        std::clog << std::string(static_cast<std::size_t>(depth + 1) * 2, ' ')
                  << "action=" << action->to_string() << '\n';
        log_tree(child.get(), depth + 2);
    }
}

std::unique_ptr<Action> MonteCarloTreeSearch::search(const GameState& state)
{
    root = std::make_shared<TreeSearchNode>(state.clone(), statistics->clone(), nullptr);
    auto new_node{root.get()};

    while(!resourcecriteria->expired())
    {
        new_node = selection->select_new(root.get());
        const auto reward = simulation->simulate(new_node->game_state());
        backup->update(new_node, reward);
    }

    if (diagnostics_enabled)
    {
        log_tree(root.get(), 0);
    }

    return bestchild->choose_action(root.get());
}