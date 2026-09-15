#pragma once
#include <iostream>
#include <sys/epoll.h>
#include <memory>

#include "Acceptor.hpp"
#include "Reactor/Reactor.hpp"
#include "Handler/ClientHandler.hpp"

Acceptor::Acceptor(std::shared_ptr<reactor::Reactor>& reactor, int server_fd): 
    reactor(reactor), server_fd(server_fd){}

void Acceptor::handle_read() {
    int clientfd = accept(server_fd, nullptr, nullptr);
    std::cout << "Accepting new client connection on " << clientfd << std::endl;

    // added client handler
    EventHandler* handler = new ClientHandler(clientfd);
    reactor->add_handler(handler);
}

void Acceptor::handle_write() {}

void Acceptor::handle_close() {}