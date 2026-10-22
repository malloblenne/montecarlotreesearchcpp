#include "simpleexpansionpolicy.h"
#include <algorithm>

using namespace mctsearch;

SimpleExpansionPolicy::SimpleExpansionPolicy(std::unique_ptr<StatisticsNode> stat): engine(r()), statistics(std::move(stat))
{

}

TreeSearchNode* SimpleExpansionPolicy::expand(TreeSearchNode* node)
{
    if (node->terminal()) return node;

    const int action_idx{get_randomly_unexplored_actionidx(node)};

    auto new_node{create_child_node_basedon_actionidx(node, action_idx)};
    auto action{node->game_state()->get_action_nth(action_idx)};

    node->add_child(new_node, action.get());

    return new_node.get();
}

int SimpleExpansionPolicy::get_randomly_unexplored_actionidx(const TreeSearchNode* node)
{
    const auto possible_actions_idx{node->unexplored_actions_idx()};
    std::vector<int> sampled;

    std::sample(possible_actions_idx.begin(), possible_actions_idx.end(), std::back_inserter(sampled),
                1, engine);
    
    return sampled.front();

}

std::shared_ptr<TreeSearchNode> SimpleExpansionPolicy::create_child_node_basedon_actionidx(TreeSearchNode* node, int action_idx) const
{
    auto state{node->game_state()->clone()};
    state->apply_nth_move(action_idx);
    return std::make_shared<TreeSearchNode>(std::move(state),statistics->clone(), node);
}