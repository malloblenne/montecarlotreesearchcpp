#pragma once
#include "gamestate.h"

namespace mctsearch
{
    class SimulationPolicy
    {
        public:
        virtual ~SimulationPolicy() = default;

        virtual WinningState simulate(const GameState* state) = 0;

        virtual WinningState simulate(const GameState* state, int player)
        {
            return simulate(state);
        }
    };
}