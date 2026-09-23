#pragma once
#include "montecarlotreesearch.h"
#include <utility>

namespace mctsearch
{
    class MonteCarloTreeSearchFactory
    {
        public:
        static std::unique_ptr<MonteCarloTreeSearch> make_SimpleMonteCarloTreeSearch();
    };
}