#include "TimerManager.hpp"

#include <functional>

void TimerManager::add_timer(int expiration, std::function<void()> task) {
    timers.emplace(expiration, task);
}

int TimerManager::ms_until_earliest_timer(){
    return timers.top().expiration;
}

void TimerManager::execute_expired_timer(){
    // call the callback
    timers.top().task();
    timers.pop();
}