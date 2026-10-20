#pragma once

#include <queue>
#include <functional>

#include "Timer.hpp"

class TimerManager {
public:
    void add_timer(int ttl, std::function<void()> task);
    int ms_until_earliest_timer();
    void execute_expired_timer();
    std::priority_queue<Timer, std::vector<Timer>, std::greater<Timer>> timers; 
};