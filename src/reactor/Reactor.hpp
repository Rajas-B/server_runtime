#pragma once

#include <sys/epoll.h>
#include <netinet/in.h>
#include <iostream>
#include <queue>
#include <mutex>
#include <memory>
#include <atomic>
#include <unordered_map>
#include <functional>
#include <vector>

#include "TimerManager.hpp"

class EventHandler;
class WakeupHandler;
class MessageQueue;
class TcpConnection;

class Reactor {
using Response = std::pair<uint64_t, std::vector<uint8_t>*>;
using WriteQueue = std::queue<Response>;

public:
    Reactor();
    void add_handler(EventHandler* handler);
    void add_to_write_ready(uint64_t conn_id, std::vector<uint8_t>* response);
    void process_write_queue();
    int get_epoll_fd();
    int start();
    void modify_epoll(int events, EventHandler* handler);
    uint64_t generate_id();
    void register_handler(uint64_t id, TcpConnection* handler);
    void unregister_handler(uint64_t id);
    TcpConnection* get_handler_if_alive(uint64_t id);
    void register_timer(int expiration, std::function<void()> task);
    void wakeup_reactor();
    void request_write_wakeup();
private:
    // this is the epoll fd
    int epoll_fd;
    std::unique_ptr<WakeupHandler> wakeup_handler;
    WriteQueue responses_to_write;
    std::mutex write_guard;
    std::atomic<uint64_t> next_connection_id{1};    
    std::unordered_map<uint64_t, TcpConnection*> active_handlers;
    std::unique_ptr<TimerManager> timer_mngr;
    std::unique_ptr<MessageQueue> message_queue;
};

