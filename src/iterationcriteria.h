#pragma once

#include "resourcecriteria.h"

#include <string>


namespace mctsearch
{
    class IterationCriteria: public ResourceCriteria
    {
        public:
        IterationCriteria(int max_iterations);

        std::string to_string() const override;
        void init() override;
        bool expired() override;

        private:
        int max_iterations;
        int iteration;
    };
}