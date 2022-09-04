#include "simplebackpropagationpolicy.h"
#include <algorithm>

using namespace mctsearch;


void SimpleBackPropagationPolicy::update(TreeSearchNode* node, WinningState reward)
{
    auto current_node{node};

    while (node != nullptr)
    {
        current_node->update_statistics(reward);
        current_node = current_node->get_parent();
    }
}