#pragma once
#include <stdint.h>
#include "ReadingCodes.hpp"

// interface for protocol frameworks to handle
class ProtocolHandler {
public:
    virtual ReadingCodes consume(const uint8_t* buf, unsigned int bytes_to_consume) = 0;
    virtual bool close_after_write() = 0;
    virtual std::vector<uint8_t>* get_message_to_write() = 0;
    virtual void on_message_sent() = 0;
    virtual void process_response(std::vector<uint8_t>* message) = 0;
    virtual void on_connection_established() = 0;
};