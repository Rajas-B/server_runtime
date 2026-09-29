#pragma once

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <queue>
#include <mutex>
#include <memory>

class EventHandler;
class WakeupHandler;

class Reactor {
public:
    Reactor();
    void add_handler(EventHandler* handler);
    void add_to_write_ready(EventHandler* handler);
    void process_write_clients();
    int get_epoll_fd();
    int start();
    void modify_epoll(int events, EventHandler* handler);
private:
    // this is the epoll fd
    int epoll_fd;
    std::unique_ptr<WakeupHandler> wakeup_handler;
    std::queue<EventHandler*> ready_to_write_clients;
    std::mutex write_guard;
};

