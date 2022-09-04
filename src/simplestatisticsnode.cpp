#include "simplestatisticsnode.h"

using namespace mctsearch;




void SimpleStatisticsNode::update(WinningState ws)
{
    count++;
    reward+= winningstate_to_double(ws);
}

double SimpleStatisticsNode::visit_count() const
{
    return count;
}

double SimpleStatisticsNode::simulation_reward() const
{
    return reward;
}

std::unique_ptr<StatisticsNode> SimpleStatisticsNode::clone() const
{
    auto cloned = std::make_unique<SimpleStatisticsNode>();
    cloned->count = count;
    cloned->reward = reward;

    return std::move(cloned);
}