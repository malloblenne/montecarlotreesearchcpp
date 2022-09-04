#pragma once

#include "treesearchnode.h"
#include "gamestate.h"


namespace mctsearch
{
    class BackPropagationPolicy
    {
        public:

        virtual ~BackPropagationPolicy() = default;

        virtual void update(TreeSearchNode* node, WinningState reward) = 0;
    };
}