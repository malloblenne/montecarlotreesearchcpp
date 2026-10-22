#pragma once

#include "statisticsnode.h"
#include "gamestate.h"


namespace mctsearch
{
    class SimpleStatisticsNode: public StatisticsNode
    {
        public:
        SimpleStatisticsNode() = default;

        void update(WinningState reward) override;
        double visit_count() const override;
        double simulation_reward() const override;
        std::unique_ptr<StatisticsNode> clone() const;

        private:

        double count = 0.0;
        double reward = 0.0;
    };
}