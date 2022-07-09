#pragma once
#include <string>

namespace mctsearch
{
    class Action
    {
        public:
        virtual std::string to_string() const = 0;
        virtual int value() const = 0;
    };
}