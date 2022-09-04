#include "treesearchnode.h"
#include <stdexcept>
#include <algorithm>

using namespace mctsearch;

TreeSearchNode::TreeSearchNode(std::unique_ptr<GameState> state,std::unique_ptr<StatisticsNode> stats, TreeSearchNode* parent):
state{std::move(state)},
statisticsnode{std::move(stats)},
parent{parent}
{
    children.resize(state->number_moves());
}

TreeSearchNode* TreeSearchNode::get_parent() const
{
    return parent;
}

bool TreeSearchNode::terminal() const
{
    return state->is_terminal();
}

bool TreeSearchNode::is_full_expanded() const
{
    if (terminal()) return true;

    return std::all_of(children.begin(), children.end(),
           [](const std::shared_ptr<mctsearch::TreeSearchNode>& n){ return n != nullptr;});
}

const GameState* TreeSearchNode::game_state() const
{
    return state.get();
}

std::vector<int> TreeSearchNode::unexplored_actions_idx() const
{
    if (children.empty()) return std::vector<int>();

    std::vector<int> actions_idx;
    actions_idx.reserve(children.size());

    for (int i = 0; i < children.size(); i++)
    {
        if (children[i] == nullptr)
        {
            actions_idx.push_back(i);
        }
    }

    return actions_idx;
}

void TreeSearchNode::add_child(std::shared_ptr<TreeSearchNode> child, Action* action)
{
    child->parent = this;
    const auto child_to_assign = action->value();

    if (child_to_assign >= children.size())
    {
        throw std::invalid_argument("Invalid Argument:" + action->to_string());
    }
     
    children[action->value()] = child;
}

std::shared_ptr<TreeSearchNode> TreeSearchNode::child(mctsearch::Action* action) const
{
    return children[action->value()];
}

std::vector<std::pair<std::unique_ptr<Action>,std::shared_ptr<TreeSearchNode>>> TreeSearchNode::children_explored() const
{
    std::vector<std::pair<std::unique_ptr<Action>,std::shared_ptr<TreeSearchNode>>> ret;
    ret.reserve(children.size());

    for (int i = 0; i < children.size(); i++)
    {
        auto child = children[i];
        if (child != nullptr)
        {
            auto action = state->get_action_nth(i);
            ret.push_back(std::make_pair(std::move(action), child));
        }
    }

    return ret;
}

const StatisticsNode* TreeSearchNode::statistics() const
{
    return statisticsnode.get();
}

void TreeSearchNode::update_statistics(WinningState reward)
{
    statisticsnode->update(reward);
}