#include "Engine.hpp"
#include "Reactor.hpp"
#include "WakeupHandler.hpp"

Server::Server() {
    reactor = std::make_unique<Reactor>();
}

Server::~Server() {
    close(wakeup_fd);
}

void Server::register_acceptor(EventHandler* acceptor) {
    reactor->add_handler(acceptor);
}

void Server::start() {
    reactor->start();
}

Reactor* Server::get_reactor() const {
    return reactor.get();
}