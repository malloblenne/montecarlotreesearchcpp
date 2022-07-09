#pragma once

#include "action.h"
#include "gamestate.h"
#include <memory>

namespace mctsearch
{
    class TreeSearch
    {
        public:
          virtual void init(std::shared_ptr<mctsearch::GameState> state) = 0;
          virtual std::unique_ptr<mctsearch::Action> search(std::shared_ptr<GameState> state) = 0;
    };
}
