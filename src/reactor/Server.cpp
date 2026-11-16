#include "Server.hpp"
#include "Reactor.hpp"
#include "WakeupHandler.hpp"
#include "Acceptor.hpp"
#include "ProtocolHandler.hpp"
#include "TcpConnection.hpp"

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

void Server::listen_on(int port, std::function<ProtocolHandler*()> protocol_handler_factory) {
    auto* reactor_ptr = reactor.get();
    auto connection_factory = [reactor_ptr, factory = std::move(protocol_handler_factory)](
                                  int fd, uint64_t conn_id) {
        return std::make_unique<TcpConnection>(
            fd, conn_id, reactor_ptr, std::unique_ptr<ProtocolHandler>(factory()));
    };

    auto acceptor = std::make_unique<Acceptor>(port, reactor_ptr, std::move(connection_factory));
    register_acceptor(acceptor.get());
    acceptors.push_back(std::move(acceptor));
}