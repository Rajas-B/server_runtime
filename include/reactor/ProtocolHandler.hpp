#pragma once
#include <stdint.h>
#include "ReadingCodes.hpp"

class ConnectionContext;
class ReactorContext;

// interface for protocol frameworks to handle
class ProtocolHandler {
public:
    virtual ReadingCodes consume(const uint8_t* buf, unsigned int bytes_to_consume) = 0;
    virtual bool close_after_write() = 0;
    virtual void on_message_sent() = 0;
    virtual std::vector<uint8_t>* return_response() = 0;
    virtual void on_connection_established(ConnectionContext* conn_ctx, ReactorContext* rctr_ctx) = 0;
    virtual void process_response_message(std::vector<uint8_t>* message) = 0;
    // notifying reactor that there is a message to write for this conn_id
    virtual void enqueue_response_and_wake() = 0;
};