#include "TcpConnection.hpp"

#include "ProtocolHandler.hpp"
#include "Reactor.hpp"

#include <arpa/inet.h>

// handle_read()?

void TcpConnection::handle_read() {
    ssize_t bytes_read = read(fd, buf, 8192);
    if (bytes_read == -1 && errno == EAGAIN) {    
        handle_close(); // Socket is drained, go back to epoll/reactor
    }
    if (bytes_read == 0) { // natural closing of connection
        graceful_shutdown();
    }
    // handing over all bytes to protocol handler
    auto code = protocol_handler->consume(buf, (unsigned int)bytes_read);
    if (code == ReadingCodes::ERROR) {
        handle_close();
    }
    else if (code == ReadingCodes::STOP) {
        pause_reading();
    }
}

void TcpConnection::handle_close() {
    close(fd);
}

void TcpConnection::graceful_shutdown() {
    shutdown(fd, SHUT_WR);
}

void TcpConnection::send_message(std::vector<uint8_t>* message) {
    current_message_to_send = message;
    handle_write();
}

// only handles write of a single message, notifies the protocol handler that the message has been sent
void TcpConnection::handle_write() {
    // fetch next message to be written from protocol handler
    // this forces ordering and queueing of messages on protocol side

    ssize_t bytes_written = write(fd, current_message_to_send->data(), current_message_to_send->size());

    if (bytes_written == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        // the OS buffer is full, can't write now
        // break;
        reactor->modify_epoll(EPOLLIN|EPOLLET|EPOLLOUT, this);
    }
    if (bytes_written < current_message_to_send->size()) {
        // partial write occurred
        // update the first current_message_to_send to contain only the remaining bytes
        current_message_to_send->erase(current_message_to_send->begin(), current_message_to_send->begin() + bytes_written);
        // break now since a partial write indicates that the OS buffer might be full
        // break;
        reactor->modify_epoll(EPOLLIN|EPOLLET|EPOLLOUT, this);
    }
    else {
        // message written by TcpConnection, notify handler
        // pause write events
        current_message_to_send = nullptr; // let the handler deal with the sent message, protocol logic is variable
        reactor->modify_epoll(EPOLLIN|EPOLLET, this);
        protocol_handler->on_message_sent();
    }
}

void TcpConnection::process_message(std::vector<uint8_t>* message) {
    protocol_handler->process_response(message);
}

uint64_t TcpConnection::get_conn_id() {
    return conn_id;
}

void TcpConnection::on_connection_established() {
    update_events_to_monitor(EPOLLIN|EPOLLET);
}

void TcpConnection::update_events_to_monitor(int events) {
    current_events_to_monitor ^= events; // difference is turned on
    reactor->modify_epoll(current_events_to_monitor, this);
}

void TcpConnection::pause_reading() {
    // update_events_to_monitor(EPOLLIN);
    current_events_to_monitor &= !EPOLLIN;
    reactor->modify_epoll(current_events_to_monitor, this);
}

void TcpConnection::resume_reading() {
    // resume reading
    current_events_to_monitor |= EPOLLIN;
    reactor->modify_epoll(current_events_to_monitor, this);
}

int TcpConnection::getfd() {
    return fd;
}