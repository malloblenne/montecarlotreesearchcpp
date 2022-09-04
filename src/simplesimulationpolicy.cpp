#include "simplesimulationpolicy.h"

using namespace mctsearch;

SimpleSimulationPolicy::SimpleSimulationPolicy():engine{r()}
{}

WinningState SimpleSimulationPolicy::simulate(const GameState* state)
{
    const auto owner{state->get_owner()};

    auto new_state{state->clone()};
    while (!new_state->is_terminal())
    {
      const auto actionidx = get_randomly_actionidx(new_state->number_moves());
      new_state->apply_nth_move(actionidx);
    }

    return new_state->score(owner);
}


int SimpleSimulationPolicy::get_randomly_actionidx(int number_of_possible_actions)
{
    std::uniform_int_distribution<int> uni(0,number_of_possible_actions-1);
    return uni(engine);
}