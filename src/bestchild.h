#pragma once

#include "treesearchnode.h"



namespace mctsearch
{
    class BestChild
    {
        public:
        TreeSearchNode* choose(TreeSearchNode* node) const;
    };
}