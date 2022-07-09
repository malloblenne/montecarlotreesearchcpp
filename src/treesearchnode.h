#pragma once

#include "action.h"
#include "gamestate.h"
#include <memory>
#include <vector>

namespace mctsearch
{
    class TreeSearchNode
    {
        public:
            TreeSearchNode(std::unique_ptr<GameState> state, std::weak_ptr<TreeSearchNode> parent);
            std::weak_ptr<TreeSearchNode> get_parent() const;
            void add_child(std::shared_ptr<TreeSearchNode> child, mctsearch::Action* action);
        
        private:
            std::vector<std::shared_ptr<TreeSearchNode>> children;
            std::unique_ptr<GameState> state;
            std::weak_ptr<TreeSearchNode> parent;
    };
}
