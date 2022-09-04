#pragma once
#include "simulationpolicy.h"
#include "gamestate.h"
#include <random>

namespace mctsearch
{
    class SimpleSimulationPolicy: public SimulationPolicy
    {
        public:
        SimpleSimulationPolicy();

        WinningState simulate(const GameState* state) override;

        private:
        int get_randomly_actionidx(int number_of_possible_actions);

        std::random_device r;
        std::default_random_engine engine;
    };
}