#include "TcpConnection.hpp"

#include "ProtocolHandler.hpp"
#include "Reactor.hpp"
#include "TcpConnectionContext.hpp"

#include <arpa/inet.h>

TcpConnection::TcpConnection(int fd, uint64_t conn_id, Reactor* reactor,
                             std::unique_ptr<ProtocolHandler> protocol_handler):
    fd(fd), conn_id(conn_id), protocol_handler(std::move(protocol_handler)), reactor(reactor) {
}

TcpConnection::~TcpConnection() = default;


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

// only handles write of a single message, notifies the protocol handler that the message has been sent
void TcpConnection::handle_write() {
    // fetch next message to be written from protocol handler
    // this forces ordering and queueing of messages on protocol side

    ssize_t bytes_written = write(fd, current_message_to_send->data(), current_message_to_send->size());
    resume_writing();
    if (bytes_written == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        // the OS buffer is full, can't write now
        // break;
    }
    else if (bytes_written < current_message_to_send->size()) {
        // partial write occurred
        // update the first current_message_to_send to contain only the remaining bytes
        current_message_to_send->erase(current_message_to_send->begin(), current_message_to_send->begin() + bytes_written);
        // break now since a partial write indicates that the OS buffer might be full
        // break;
    }
    else {
        // message written by TcpConnection, notify handler
        // pause write events
        current_message_to_send->erase(current_message_to_send->begin(), current_message_to_send->begin() + current_message_to_send->size());
        current_message_to_send = nullptr; // let the handler deal with the sent message, protocol logic is variable
        reactor->modify_epoll(EPOLLIN|EPOLLET, this);
        protocol_handler->on_message_sent();
    }
}

void TcpConnection::get_message_and_write() {
    current_message_to_send = protocol_handler->return_response();
    handle_write();
}

uint64_t TcpConnection::get_conn_id() {
    return conn_id;
}

void TcpConnection::on_connection_established() {
    // add protocol handler initialization
    protocol_handler->on_connection_established(new TcpConnectionContext(this));
    current_events_to_monitor = EPOLLIN|EPOLLET;
}

int TcpConnection::getfd() {
    return fd;
}

// reactor context
void TcpConnection::pause_reading() {
    // update_events_to_monitor(EPOLLIN);
    if (current_events_to_monitor&EPOLLIN == 0) {
        return;
    }
    current_events_to_monitor &= !EPOLLIN;
    reactor->modify_epoll(current_events_to_monitor, this);
}

void TcpConnection::resume_reading() {
    // resume reading
    if (current_events_to_monitor&EPOLLIN > 0) {
        return;
    }
    current_events_to_monitor |= EPOLLIN;
    reactor->modify_epoll(current_events_to_monitor, this);
}

void TcpConnection::resume_writing() {
    if (current_events_to_monitor&EPOLLOUT > 0) {
        return;
    }
    current_events_to_monitor |= EPOLLOUT;
    reactor->modify_epoll(current_events_to_monitor, this);
}

void TcpConnection::pause_writing() {
    if (current_events_to_monitor&EPOLLOUT == 0) {
        return;
    }
    current_events_to_monitor |= EPOLLOUT;
    reactor->modify_epoll(current_events_to_monitor, this);
}