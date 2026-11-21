#pragma once
#include "EventContext.hpp"

class ProtocolHandler;
class Reactor;

class ReactorEventContext: public EventContext {
public:
    void request_write_wakeup() override;
    void remove_write_wakeup() override;
    void add_to_write_queue() override;
    void bind_handler(ProtocolHandler* client_handler) override;
private:
    Reactor& reactor;
    ProtocolHandler* handler;
};