#pragma once

#include "treesearchnode.h"



namespace mctsearch
{
    class SelectionPolicy
    {
        public:
        virtual ~SelectionPolicy() = default;

        virtual TreeSearchNode* select_new(TreeSearchNode* node) const = 0;

    };
}