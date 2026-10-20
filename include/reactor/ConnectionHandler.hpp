#pragma once
#include "EventHandler.hpp"
#include <stdint.h>

class ConnectionHandler: public EventHandler {
public:
    virtual bool ready_for_write() = 0;
    virtual void mark_as_processed() = 0;
    virtual std::chrono::steady_clock::time_point get_last_active_timestamp() = 0;
    virtual int get_ttl() = 0;
    virtual void handle_timeout() = 0;
    virtual uint64_t get_conn_id() = 0;
    virtual void on_connection_established() = 0; // add timeouts and other related tasks
};