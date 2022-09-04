#pragma once

#include "bestchild.h"

namespace mctsearch
{
    class UpperConfidenceBoundBestChild: public BestChild
    {
        public:
        UpperConfidenceBoundBestChild(double weight);

        TreeSearchNode* choose(TreeSearchNode* node) const override;
        std::unique_ptr<Action> choose_action(TreeSearchNode* node) const override;
        virtual std::unique_ptr<Action> choose_action_exploitation_only(TreeSearchNode* node) const override;

        private:

        std::unique_ptr<Action> choose_action(TreeSearchNode* node, double w) const;

        double uct_value(const StatisticsNode* s_node, const StatisticsNode* s_child, double w) const;

        // Balance exploration and exploitation
        double weight;
    };
}