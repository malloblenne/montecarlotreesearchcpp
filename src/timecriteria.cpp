#include "timecriteria.h"

#include <chrono>
 
using namespace mctsearch;
using namespace std::chrono_literals;



TimeCriteria::TimeCriteria(std::chrono::duration<int> duration): duration{duration}
{

}

std::string TimeCriteria::to_string() const
{
    return _expired() ? "Time expired" : "Time still available";
}

void TimeCriteria::init()
{
    start = std::chrono::steady_clock::now();
}

bool TimeCriteria::expired()
{
    return _expired();
}

bool TimeCriteria::_expired() const
{
    return (std::chrono::steady_clock::now() - start) > duration;
}