#pragma once

#include "action.h"
#include "treesearch.h"
#include "treesearchnode.h"
#include "selectionpolicy.h"
#include "expansionpolicy.h"
#include "backpropagationpolicy.h"
#include "simulationpolicy.h"
#include "statisticsnode.h"
#include "resourcecriteria.h"
#include "bestchild.h"

namespace mctsearch
{
    class MonteCarloTreeSearch: public TreeSearch
    {
        public:
            struct MonteCarloTreeSearchInitialization
            {
                std::unique_ptr<SelectionPolicy> selection;
                std::unique_ptr<BackPropagationPolicy> backup;
                std::unique_ptr<SimulationPolicy> simulation;
                std::unique_ptr<ResourceCriteria> resourcecriteria;
                std::unique_ptr<BestChild> bestchild;
                std::unique_ptr<StatisticsNode> statistics;
            };

            MonteCarloTreeSearch(MonteCarloTreeSearchInitialization init);
            void init() override;
            std::unique_ptr<Action> search(const GameState& state) override;
        private:
            std::unique_ptr<SelectionPolicy> selection;
            std::unique_ptr<ExpansionPolicy> expansion;
            std::unique_ptr<BackPropagationPolicy> backup;
            std::unique_ptr<SimulationPolicy> simulation;
            std::unique_ptr<ResourceCriteria> resourcecriteria;
            std::unique_ptr<BestChild> bestchild;
            std::unique_ptr<StatisticsNode> statistics;
            
            std::shared_ptr<TreeSearchNode> root;
    };
}
