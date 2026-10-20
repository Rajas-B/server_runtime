#pragma once

#include "EventHandler.hpp"


#include <memory>
#include <sys/socket.h>

class EventContext;
class ReactorEventContext;
class Reactor;
class TcpConnection;
class Connection;
class ConnectionHandler;

template <typename TFactory>
class Acceptor: public EventHandler {
public:
    int getfd () override {
        return server_fd;
    }
    void setup_listening_fd() {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        bind(server_fd, (sockaddr*)&addr, sizeof(addr));
        listen(server_fd, SOMAXCONN);
    }
    

    void bind_to_port() {
        server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        int val = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    }

    void handle_read() override {
        int clientfd = accept4(server_fd, nullptr, nullptr, SOCK_NONBLOCK);
        
        // added client handler
        uint64_t conn_id = reactor->generate_id();
        std::unique_ptr<EventContext> context = std::make_unique<ReactorEventContext>(*reactor);
        std::unique_ptr<Connection> conn = std::make_unique<TcpConnection>(clientfd, conn_id);
        ConnectionHandler* handler = handler_factory(std::move(conn), std::move(context));
        reactor->register_handler(conn_id, handler);
        reactor->add_handler(handler);
        handler->on_connection_established();
    }

    void handle_write() override {}

    void handle_close() override {}

    Acceptor(int port, Reactor* reactor, TFactory factory): 
        port(port), reactor(reactor), handler_factory(factory) {
        setup_listening_fd();
        bind_to_port();
    }


private:
    int server_fd;
    int port;
    Reactor* reactor;
    TFactory handler_factory;
};