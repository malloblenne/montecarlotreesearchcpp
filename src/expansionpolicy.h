#pragma once
#include "treesearchnode.h"

namespace mctsearch
{
    class ExpansionPolicy
    {
        public:
        virtual ~ExpansionPolicy() = default;
        virtual TreeSearchNode* expand(TreeSearchNode* node) = 0;
    };
}