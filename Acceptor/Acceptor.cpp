#pragma once
#include <iostream>
#include <sys/epoll.h>
#include <memory>

#include "Acceptor.hpp"
#include "Reactor/Reactor.hpp"
#include "HTTP/v1.1/HTTPParser.hpp"
#include "Handler/ClientHandler.hpp"
#include "Protocol/IParserFactory.hpp"


Acceptor::Acceptor(int server_fd, reactor::Reactor* reactor, int wakeup_fd): 
    server_fd(server_fd), reactor(reactor), wakeup_fd(wakeup_fd) {}

void Acceptor::handle_read() {
    int clientfd = accept(server_fd, nullptr, nullptr);
    std::cout << "Accepting new client connection on " << clientfd << std::endl;

    // added client handler
    EventHandler* handler = new ClientHandler(clientfd, new HTTPParser(), wakeup_fd, reactor);
    reactor->add_handler(handler);
}

void Acceptor::handle_write() {}

void Acceptor::handle_close() {}