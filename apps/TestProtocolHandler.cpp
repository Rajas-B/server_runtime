#include "TestProtocolHandler.hpp"

ReadingCodes TestProtocolHandler::consume(const uint8_t* buf, unsigned int bytes_to_consume) {
}

bool TestProtocolHandler::close_after_write() {
}

void TestProtocolHandler::on_message_sent() {
}

void TestProtocolHandler::process_response(std::vector<uint8_t>* message) {
}

void TestProtocolHandler::on_connection_established(ConnectionContext* conn_ctx) {
    
}
