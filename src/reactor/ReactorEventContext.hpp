#pragma once
#include "EventContext.hpp"

class ConnectionHandler;
class Reactor;

class ReactorEventContext: public EventContext {
public:
    void request_write_wakeup() override;
    void remove_write_wakeup() override;
    void add_to_write_queue() override;
    void bind_handler(ConnectionHandler* client_handler) override;
    // function to add a timer of a handler (ttl + now)
    // function to update a timer of a handler (ttl + now)
    void add_timer(int expiration, std::function<void()> task) override;
private:
    Reactor& reactor;
    ConnectionHandler* handler;
};