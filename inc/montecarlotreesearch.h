#pragma once

#include "treesearch.h"
#include "treesearchnode.h"
#include "selectionpolicy.h"
#include "expansionpolicy.h"
#include "backpropagationpolicy.h"
#include "simulationpolicy.h"
#include "resourcecriteria.h"

namespace MonteCarloTreeSearch
{
    class MonteCarloTreeSearch: public TreeSearch
    {
        public:
            struct MonteCarloTreeSearchInitialization
            {
                std::unique_ptr<SelectionPolicy> selection;
                std::unique_ptr<ExpansionPolicy> expansion;
                std::unique_ptr<BackPropagationPolicy> backup;
                std::unique_ptr<SimulationPolicy> simulation;
                std::unique_ptr<ResourceCriteria> resourcecriteria;
            };

            MonteCarloTreeSearch(MonteCarloTreeSearchInitialization init);
            void init(std::shared_ptr<GameState> state) override;
            std::unique_ptr<Action> search(std::shared_ptr<GameState> state) override;
        private:
            std::unique_ptr<SelectionPolicy> selection;
            std::unique_ptr<ExpansionPolicy> expansion;
            std::unique_ptr<BackPropagationPolicy> backup;
            std::unique_ptr<SimulationPolicy> simulation;
            std::unique_ptr<ResourceCriteria> resourcecriteria;
            
            std::shared_ptr<TreeSearchNode> root;
    };
}
