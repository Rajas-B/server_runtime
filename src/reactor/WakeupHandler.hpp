#pragma once
#include "EventHandler.hpp"

class Reactor;

class WakeupHandler: public EventHandler {
public:
    void handle_read() override;
    void handle_write() override;
    int getfd() override;
    WakeupHandler(int wakeup_fd, Reactor* reactor): wakeup_fd(wakeup_fd), reactor(reactor) {}
private:
    int wakeup_fd;
    Reactor* reactor;
};