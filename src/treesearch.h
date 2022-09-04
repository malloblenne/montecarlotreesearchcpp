#pragma once

#include "action.h"
#include "gamestate.h"
#include <memory>

namespace mctsearch
{
    class TreeSearch
    {
        public:
          virtual ~TreeSearch() = default;
          
          virtual void init() = 0;
          virtual std::unique_ptr<mctsearch::Action> search(const GameState& state) = 0;
    };
}
