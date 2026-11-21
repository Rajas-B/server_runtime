#pragma once

#include "ProtocolHandler.hpp"
#include "ReadingCodes.hpp"

#include <vector>
#include <stdint.h>

class ConnectionContext;

class TestProtocolHandler : public ProtocolHandler {
public:
    ReadingCodes consume(const uint8_t* buf, unsigned int bytes_to_consume) override;
    bool close_after_write() override;
    void on_message_sent() override;
    void process_response(std::vector<uint8_t>* message) override;
    void on_connection_established(ConnectionContext* conn_ctx) override;
private:
    std::vector<
};