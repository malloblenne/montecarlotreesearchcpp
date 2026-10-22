#pragma once

#include "expansionpolicy.h"
#include "treesearchnode.h"
#include "statisticsnode.h"
#include <random>
#include <memory>

namespace mctsearch
{

    class SimpleExpansionPolicy: public ExpansionPolicy
    {
        public:
        SimpleExpansionPolicy(std::unique_ptr<StatisticsNode> statistics);

        TreeSearchNode* expand(TreeSearchNode* node) override;

        private:

        int get_randomly_unexplored_actionidx(const TreeSearchNode* node);

        std::shared_ptr<TreeSearchNode> create_child_node_basedon_actionidx(TreeSearchNode* node, int action_idx) const;

        std::random_device r;
        std::default_random_engine engine;
        std::unique_ptr<StatisticsNode> statistics;
    };
}