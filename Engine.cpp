#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <memory>

#include "Logger/Logger.hpp"
#include "Reactor/Reactor.hpp"
#include "Acceptor/Acceptor.hpp"

int main() {
    // create runtime, as a static object?
    // start it
    
    // set up non blocking listening socket
    int server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    int val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    
    // bind to port 8080
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, SOMAXCONN);

    // create epoll instance in OS
    // which is managed by the reactorrrr
    auto reactor = std::make_shared<reactor::Reactor>(server_fd, 8080);
    Acceptor acceptor(reactor, server_fd);
    reactor->add_handler(&acceptor);
    reactor->start();


}