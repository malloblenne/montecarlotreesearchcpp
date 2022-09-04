#pragma once

#include "selectionpolicy.h"
#include "expansionpolicy.h"
#include "bestchild.h"



namespace mctsearch
{
    class SimpleSelectionPolicy: public SelectionPolicy
    {
        public:
        SimpleSelectionPolicy(std::unique_ptr<ExpansionPolicy> e, std::unique_ptr<BestChild> b);
        TreeSearchNode* select_new(TreeSearchNode* node) const override;

        private:
        std::unique_ptr<ExpansionPolicy> expansion;
        std::unique_ptr<BestChild> bestchild;
    };
}
