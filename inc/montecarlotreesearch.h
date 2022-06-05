#pragma once

#include "treesearch.h"
#include "treesearchnode.h"
#include "selectionpolicy.h"
#include "expansionpolicy.h"
#include "backpropagationpolicy.h"
#include "simulationpolicy.h"

#include <chrono>

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
                std::chrono::duration<int> duration;
            };

            MonteCarloTreeSearch(MonteCarloTreeSearchInitialization init);
            void init(std::shared_ptr<GameState> state) override;
            std::unique_ptr<Action> search(std::shared_ptr<GameState> state) override;
        private:
            std::unique_ptr<SelectionPolicy> selection;
            std::unique_ptr<ExpansionPolicy> expansion;
            std::unique_ptr<BackPropagationPolicy> backup;
            std::unique_ptr<SimulationPolicy> simulation;
            std::chrono::duration<int> duration;
            
            std::shared_ptr<TreeSearchNode> root;
    };
}
