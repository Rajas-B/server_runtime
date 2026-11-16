#pragma once

#include "EventHandler.hpp"

#include <functional>
#include <memory>
#include <stdint.h>
#include <sys/socket.h>

class Reactor;
class TcpConnection;

class Acceptor: public EventHandler {
public:
    int getfd () override;
    void setup_listening_fd();
    void bind_to_port();
    void handle_read() override;
    void handle_write() override {}
    void handle_close() override {}
    Acceptor(int port, Reactor* reactor,
             std::function<std::unique_ptr<TcpConnection>(int, uint64_t)> connection_factory);


private:
    int server_fd;
    int port;
    Reactor* reactor;
    std::function<std::unique_ptr<TcpConnection>(int, uint64_t)> connection_factory;
};