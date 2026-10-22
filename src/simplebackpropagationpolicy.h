#pragma once

#include "backpropagationpolicy.h"

namespace mctsearch
{
    class SimpleBackPropagationPolicy: public BackPropagationPolicy
    {
        public:

        SimpleBackPropagationPolicy() = default;

        void update(TreeSearchNode* node, WinningState reward) override;
    };
}