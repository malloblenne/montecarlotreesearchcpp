#include "montecarlotreesearchfactory.h"
#include "simpleexpansionpolicy.h"
#include "simpleselectionpolicy.h"
#include "simplebackpropagationpolicy.h"
#include "simplesimulationpolicy.h"
#include "timecriteria.h"
#include "iterationcriteria.h"
#include "upperconfidenceboundbestchild.h"
#include "simplestatisticsnode.h"

using namespace mctsearch;
using namespace std;
using namespace std::chrono_literals;

namespace mctsearch
{
    std::unique_ptr<MonteCarloTreeSearch> MonteCarloTreeSearchFactory::make_SimpleMonteCarloTreeSearch()
    {
        const double weight = 0.5;
        //const std::chrono::milliseconds max_allowed_time = 2000ms;
        const int max_iterations = 10000;

        MonteCarloTreeSearch::MonteCarloTreeSearchInitialization init{
                .selection{make_unique<SimpleSelectionPolicy>(make_unique<SimpleExpansionPolicy>(make_unique<SimpleStatisticsNode>()),make_unique<UpperConfidenceBoundBestChild>(weight))},
                .backup{make_unique<SimpleBackPropagationPolicy>()},
                .simulation{make_unique<SimpleSimulationPolicy>()},
                //.resourcecriteria{make_unique<TimeCriteria>(max_allowed_time)},
                .resourcecriteria{make_unique<IterationCriteria>(max_iterations)},
                .bestchild{make_unique<UpperConfidenceBoundBestChild>(0)},
                .statistics{make_unique<SimpleStatisticsNode>()}
            };

        return make_unique<MonteCarloTreeSearch>(std::move(init));
    }
}