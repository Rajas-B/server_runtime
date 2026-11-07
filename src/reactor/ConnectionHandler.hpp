#pragma once
#include "EventHandler.hpp"
#include <stdint.h>
#include <vector>

class ConnectionHandler: public EventHandler {
public:
    // virtual void mark_as_processed() = 0;
    virtual std::chrono::steady_clock::time_point get_last_active_timestamp() = 0;
    // virtual int get_ttl() = 0;
    // virtual void handle_timeout() = 0;
    virtual uint64_t get_conn_id() = 0;
    virtual void on_connection_established() = 0; // add timeouts and other related tasks
    virtual void graceful_shutdown() = 0;
    virtual void pause_reading() = 0;
    virtual void resume_reading() = 0;
    virtual void send_message(std::vector<uint8_t>* message) = 0;
};