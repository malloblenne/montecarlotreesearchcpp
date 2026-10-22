#include "catch2/catch.hpp"
#include "iterationcriteria.h"
#include "simplestatisticsnode.h"
#include "montecarlotreesearchfactory.h"
#include "simplebackpropagationpolicy.h"
#include "simpleexpansionpolicy.h"
#include "simplesimulationpolicy.h"
#include "test_game_state.h"
#include "treesearchnode.h"
#include "upperconfidenceboundbestchild.h"

using namespace mctsearch;

TEST_CASE("Statistics accumulate rewards and visits", "[library][statistics]")
{
    SimpleStatisticsNode statistics;
    REQUIRE(statistics.visit_count() == 0);
    REQUIRE(statistics.simulation_reward() == 0);
    statistics.update(WinningState::WIN);
    statistics.update(WinningState::DRAW);
    statistics.update(WinningState::LOSS);
    statistics.update(WinningState::WIN);
    REQUIRE(statistics.visit_count() == 4);
    REQUIRE(statistics.simulation_reward() == 1);
}

TEST_CASE("Statistics clones preserve values independently", "[library][statistics]")
{
    SimpleStatisticsNode original;
    original.update(WinningState::WIN);
    auto copy = original.clone();
    REQUIRE(copy->visit_count() == 1);
    REQUIRE(copy->simulation_reward() == 1);
    copy->update(WinningState::LOSS);
    REQUIRE(copy->visit_count() == 2);
    REQUIRE(copy->simulation_reward() == 0);
    REQUIRE(original.visit_count() == 1);
    REQUIRE(original.simulation_reward() == 1);
}

TEST_CASE("Iteration criteria expire on the configured check and reset", "[library][criteria]")
{
    IterationCriteria criteria(3);
    REQUIRE_FALSE(criteria.expired());
    REQUIRE_FALSE(criteria.expired());
    REQUIRE(criteria.expired());
    REQUIRE(criteria.expired());
    criteria.init();
    REQUIRE(criteria.to_string() == "Iteration: 0 of 3");
    REQUIRE_FALSE(criteria.expired());
}

TEST_CASE("Tree nodes track children and unexplored actions", "[library][tree]")
{
    TreeSearchNode root(std::make_unique<TestGameState>(1, 2),
                        std::make_unique<SimpleStatisticsNode>(), nullptr);
    REQUIRE(root.get_parent() == nullptr);
    REQUIRE_FALSE(root.terminal());
    REQUIRE_FALSE(root.is_full_expanded());
    REQUIRE(root.unexplored_actions_idx() == std::vector<int>{0, 1});
    REQUIRE(root.children_explored().empty());

    for (int index = 0; index < 2; ++index)
    {
        auto state = root.game_state()->clone();
        state->apply_nth_move(index);
        auto child = std::make_shared<TreeSearchNode>(std::move(state),
                      std::make_unique<SimpleStatisticsNode>(), nullptr);
        TestAction action(index);
        root.add_child(child, &action);
        REQUIRE(root.child(&action) == child);
        REQUIRE(child->get_parent() == &root);
        REQUIRE(child->terminal());
        REQUIRE(child->is_full_expanded());
        REQUIRE(child->unexplored_actions_idx().empty());
        REQUIRE(root.children_explored().size() == static_cast<std::size_t>(index + 1));
    }
    REQUIRE(root.is_full_expanded());
    REQUIRE(root.unexplored_actions_idx().empty());
    TestAction invalid(2);
    REQUIRE_THROWS_AS(root.add_child(std::make_shared<TreeSearchNode>(
        std::make_unique<TestGameState>(0), std::make_unique<SimpleStatisticsNode>(), nullptr),
        &invalid), std::invalid_argument);
}

TEST_CASE("Backpropagation updates every ancestor", "[library][backup]")
{
    TreeSearchNode root(std::make_unique<TestGameState>(),
                        std::make_unique<SimpleStatisticsNode>(), nullptr);
    TreeSearchNode child(std::make_unique<TestGameState>(1),
                         std::make_unique<SimpleStatisticsNode>(), &root);
    TreeSearchNode leaf(std::make_unique<TestGameState>(0),
                        std::make_unique<SimpleStatisticsNode>(), &child);
    SimpleBackPropagationPolicy backup;
    backup.update(&leaf, WinningState::WIN);
    backup.update(&leaf, WinningState::LOSS);
    for (const auto* node : {&root, &child, &leaf})
    {
        REQUIRE(node->statistics()->visit_count() == 2);
        REQUIRE(node->statistics()->simulation_reward() == 0);
    }
}

TEST_CASE("Expansion creates a legal child without mutating the parent", "[library][expansion]")
{
    TreeSearchNode root(std::make_unique<TestGameState>(1),
                        std::make_unique<SimpleStatisticsNode>(), nullptr);
    SimpleExpansionPolicy expansion(std::make_unique<SimpleStatisticsNode>());
    auto* child = expansion.expand(&root);
    REQUIRE(child->terminal());
    REQUIRE(child->get_parent() == &root);
    REQUIRE(child->statistics()->visit_count() == 0);
    REQUIRE(root.game_state()->to_string() == "1");
    REQUIRE(root.is_full_expanded());
    REQUIRE(expansion.expand(child) == child);
}

TEST_CASE("Simulation scores a forced path from the requested perspective", "[library][simulation]")
{
    TestGameState state(3);
    SimpleSimulationPolicy simulation;
    REQUIRE(simulation.simulate(&state) == WinningState::WIN);
    REQUIRE(simulation.simulate(&state, 2) == WinningState::LOSS);
    REQUIRE(state.to_string() == "3");
    REQUIRE(state.get_owner() == 1);
    TestGameState terminal(0);
    REQUIRE(simulation.simulate(&terminal) == WinningState::WIN);
}

TEST_CASE("UCB selects the higher reward at equal visit counts", "[library][ucb]")
{
    TreeSearchNode root(std::make_unique<TestGameState>(1, 2),
                        std::make_unique<SimpleStatisticsNode>(), nullptr);
    for (int index = 0; index < 2; ++index)
    {
        auto child = std::make_shared<TreeSearchNode>(std::make_unique<TestGameState>(0),
                      std::make_unique<SimpleStatisticsNode>(), &root);
        child->update_statistics(index == 0 ? WinningState::LOSS : WinningState::WIN);
        root.update_statistics(WinningState::WIN);
        TestAction action(index);
        root.add_child(child, &action);
    }
    UpperConfidenceBoundBestChild best(0);
    REQUIRE(best.choose_action(&root)->value() == 1);
    REQUIRE(best.choose_action_exploitation_only(&root)->value() == 1);
    TestAction action(1);
    REQUIRE(best.choose(&root) == root.child(&action).get());
    REQUIRE_THROWS_AS(UpperConfidenceBoundBestChild(-0.1), std::invalid_argument);
    REQUIRE_THROWS_AS(UpperConfidenceBoundBestChild(1.1), std::invalid_argument);
}

TEST_CASE("Factory search returns a legal action and can be reset", "[library][search]")
{
    TestGameState state(3);
    auto search = MonteCarloTreeSearchFactory::make_SimpleMonteCarloTreeSearch(20);
    for (int attempt = 0; attempt < 2; ++attempt)
    {
        search->init();
        auto action = search->search(state);
        REQUIRE(action != nullptr);
        REQUIRE(action->value() == 0);
        REQUIRE(state.to_string() == "3");
        REQUIRE(state.get_owner() == 1);
    }
}