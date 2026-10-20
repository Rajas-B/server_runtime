#pragma once
#include "Connection.hpp"

class TcpConnection: public Connection {
    int fd;
    uint64_t conn_id;

public:
    ssize_t read_from_connection(uint8_t* buf, size_t max_length) override;
    ssize_t write_to_connection(const uint8_t* buf, size_t message_length) override;
    
    void close_connection() override;
    
    uint64_t get_conn_id() const override;

    TcpConnection(int fd_, uint64_t conn_id_);
    void graceful_shutdown() override;
};