#pragma once

#include "EventHandler.hpp"

#include <memory>
#include <sys/socket.h>

class EventContext;
class ReactorEventContext;
class Reactor;

template <typename THandler>
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
        int clientfd = accept(server_fd, nullptr, nullptr);
        
        // added client handler
        std::unique_ptr<EventContext> context = std::make_unique<ReactorEventContext>(*reactor);
        EventHandler* handler = new THandler(clientfd, std::move(context));
        reactor->add_handler(handler);
    }
    Acceptor(int port, Reactor* reactor): 
        port(port), reactor(reactor) {
        setup_listening_fd();
        bind_to_port();
    }


private:
    int server_fd;
    int port;
    Reactor* reactor;
};