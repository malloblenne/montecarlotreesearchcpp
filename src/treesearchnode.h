#pragma once

#include "action.h"
#include "gamestate.h"
#include "statisticsnode.h"
#include <memory>
#include <vector>

namespace mctsearch
{
    class TreeSearchNode
    {
        public:
            TreeSearchNode(std::unique_ptr<GameState> state, std::unique_ptr<StatisticsNode> stats, TreeSearchNode* parent);
            TreeSearchNode* get_parent() const;
            void add_child(std::shared_ptr<TreeSearchNode> child, mctsearch::Action* action);
            std::shared_ptr<TreeSearchNode> child(mctsearch::Action* action) const;
            bool terminal() const;
            bool is_full_expanded() const;
            const GameState* game_state() const;
            std::vector<int> unexplored_actions_idx() const;
            std::vector<std::pair<std::unique_ptr<Action>,std::shared_ptr<TreeSearchNode>>> children_explored() const;
            const StatisticsNode* statistics() const;
            void update_statistics(WinningState reward);
        
        private:
            std::vector<std::shared_ptr<TreeSearchNode>> children;
            std::unique_ptr<GameState> state;
            std::unique_ptr<StatisticsNode> statisticsnode;
            TreeSearchNode* parent;
    };
}
