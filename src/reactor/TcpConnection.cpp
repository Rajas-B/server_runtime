#include "TcpConnection.hpp"

#include <arpa/inet.h>

ssize_t TcpConnection::read_from_connection(uint8_t* buf, size_t max_length) {
    return read(fd, buf, max_length);
}

ssize_t TcpConnection::write_to_connection(const uint8_t* buf, size_t message_length) {
    return write(fd, buf, message_length);
}

void TcpConnection::close_connection() {
    close(fd);
}

uint64_t TcpConnection::get_conn_id() const {
    return conn_id;
}

TcpConnection::TcpConnection(int fd_, uint64_t conn_id_):
 fd(fd_), conn_id(conn_id_) {}

void TcpConnection::graceful_shutdown() {
    shutdown(fd, SHUT_WR);
}