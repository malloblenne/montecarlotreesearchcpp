#include "treesearchnode.h"
#include <stdexcept>

using namespace mctsearch;

TreeSearchNode::TreeSearchNode(std::unique_ptr<GameState> state,  std::weak_ptr<TreeSearchNode> parent):
state{std::move(state)},
parent{parent}
{
    children.resize(state->number_moves());
}

std::weak_ptr<TreeSearchNode> TreeSearchNode::get_parent() const
{
    return parent;
}

void TreeSearchNode::add_child(std::shared_ptr<TreeSearchNode> child, Action* action)
{
    const auto child_to_assign = action->value();

    if (child_to_assign >= children.size())
    {
        throw std::invalid_argument("Invalid Argument:" + action->to_string());
    }
     
    children[action->value()] = child;
}