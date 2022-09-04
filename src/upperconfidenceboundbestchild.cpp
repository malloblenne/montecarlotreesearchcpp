#include "upperconfidenceboundbestchild.h"
#include <random>
#include <algorithm>
#include <cmath>
#include <string>

using namespace mctsearch;

UpperConfidenceBoundBestChild::UpperConfidenceBoundBestChild(double w):
weight{w}
{
    if (weight > 1.0 || weight < 0.0)
    {
        throw std::invalid_argument(std::string("Invalid Argument: weight should be (0.0, 1.0), instead ") + std::to_string(weight));
    }
}

TreeSearchNode* UpperConfidenceBoundBestChild::choose(TreeSearchNode* node) const
{
    const auto action{choose_action(node, this->weight)};
    return node->child(action.get()).get();
}

std::unique_ptr<Action> UpperConfidenceBoundBestChild::choose_action(TreeSearchNode* node) const
{
    return choose_action(node, this->weight);
}

std::unique_ptr<Action> UpperConfidenceBoundBestChild::choose_action_exploitation_only(TreeSearchNode* node) const
{
    return choose_action(node, 0.0);
}

std::unique_ptr<Action> UpperConfidenceBoundBestChild::choose_action(TreeSearchNode* node, double w) const
{
    auto actions_and_children = node->children_explored();

    auto stat = node->statistics();

    auto result = std::max_element(actions_and_children.begin(), actions_and_children.end(), 
                      [stat,this](auto& a, auto& b){return uct_value(stat, a.second->statistics(), weight) < uct_value(stat, b.second->statistics(),weight);});

    if (result != std::end(actions_and_children))
    {
        return std::move(result->first);
    }
    else
    {
        throw std::logic_error("It doesn't return child that maximize UTC score");
    }

}


double UpperConfidenceBoundBestChild::uct_value(const StatisticsNode* s_node, const StatisticsNode* s_child, double w) const
{
    return s_child->simulation_reward() / s_child->visit_count() + 
           w * std::sqrt(2.0* std::log(s_node->visit_count())/s_child->visit_count());
}