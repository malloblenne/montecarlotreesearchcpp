#include "iterationcriteria.h"

using namespace mctsearch;

IterationCriteria::IterationCriteria(int max):
max_iterations{max},
iteration{0}
{

}

std::string IterationCriteria::to_string() const
{
    return std::string("Iteration: ") + std::to_string(iteration) +
           std::string(" of ") + std::to_string(max_iterations);
}

void IterationCriteria::init()
{
    iteration = 0;
}

bool IterationCriteria::expired()
{
    iteration++; //assumption every cycle we call it
    return iteration >= max_iterations;
}
