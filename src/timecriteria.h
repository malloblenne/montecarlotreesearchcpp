#pragma once

#include "resourcecriteria.h"

#include <string>
#include <chrono>



namespace mctsearch
{
    class TimeCriteria: public ResourceCriteria
    {
        public:
        TimeCriteria(std::chrono::duration<int> duration);

        std::string to_string() const override;
        void init() override;
        bool expired() const override;

        private:
        std::chrono::duration<int> duration;
        std::chrono::steady_clock::time_point start;
    };
}