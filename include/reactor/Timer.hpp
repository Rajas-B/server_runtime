#pragma once

#include <stdint.h>
#include <functional>

struct Timer {
    int expiration;
    std::function<void()> task;
    Timer(int expiration_, std::function<void()> task_):
        expiration(expiration_), task(task_) {}

    bool operator>(const Timer& other) const {
        return expiration > other.expiration;
    }
};