#include "Acceptor.hpp"
#include "ProtocolHandler.hpp"
#include "TcpConnection.hpp"
#include "Reactor.hpp"

int Acceptor::getfd () override {
    return server_fd;
}

void Acceptor::setup_listening_fd() {
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, SOMAXCONN);
}

void Acceptor::bind_to_port() {
    server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    int val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
}

void Acceptor::handle_read() override {
    int clientfd = accept4(server_fd, nullptr, nullptr, SOCK_NONBLOCK);
    uint64_t conn_id = reactor->generate_id();
    // TODO
    // make tcp connection
    // feed the tcp connection a protocol handler
    
    // register the connection with the reactor and add it to epoll
    // reactor->register_handler(conn_id, handler);
    // reactor->add_handler(handler);
    // handler->on_connection_established();

}

void Acceptor::handle_write() override {}

void Acceptor::handle_close() override {}

Acceptor::Acceptor(int port, Reactor* reactor,
                   std::function<std::unique_ptr<TcpConnection>(int, uint64_t)> connection_factory):
    port(port), reactor(reactor), connection_factory(std::move(connection_factory)) {
    setup_listening_fd();
    bind_to_port();
}