#pragma once
#include <string>

namespace MonteCarloTreeSearch
{
    class ResourceCriteria
    {
        public:
        virtual std::string to_string() const = 0;
        virtual void init() = 0;
        virtual bool expired() const = 0;
    };
}