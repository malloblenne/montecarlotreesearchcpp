#pragma once

#include "gamestate.h"
#include <memory>

namespace mctsearch
{
    class StatisticsNode
    {
        public:
        virtual ~StatisticsNode() = default;
        virtual void update(WinningState reward) = 0;
        virtual double visit_count() const = 0;
        virtual double simulation_reward() const = 0;
        virtual std::unique_ptr<StatisticsNode> clone() const = 0;
    };
}