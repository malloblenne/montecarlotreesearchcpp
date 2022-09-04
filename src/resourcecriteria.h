#pragma once
#include <string>

namespace mctsearch
{
    class ResourceCriteria
    {
        public:
        virtual ~ResourceCriteria() = default;
        
        virtual std::string to_string() const = 0;
        virtual void init() = 0;
        virtual bool expired() const = 0;
    };
}