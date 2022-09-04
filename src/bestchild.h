#pragma once

#include "treesearchnode.h"
#include "action.h"


namespace mctsearch
{
    class BestChild
    {
        public:
        virtual ~BestChild() = default;
        virtual TreeSearchNode* choose(TreeSearchNode* node) const = 0;
        virtual std::unique_ptr<Action> choose_action(TreeSearchNode* node) const = 0;
        virtual std::unique_ptr<Action> choose_action_exploitation_only(TreeSearchNode* node) const = 0;
    };
}