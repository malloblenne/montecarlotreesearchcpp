#pragma once

#include "action.h"
#include "gamestate.h"
#include <memory>

namespace MonteCarloTreeSearch
{
    class TreeSearch
    {
        public:
          virtual void init(std::shared_ptr<GameState> state) = 0;
          virtual std::unique_ptr<Action> search(std::shared_ptr<GameState> state) = 0;
    };
}
