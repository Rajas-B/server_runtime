#include "Engine.hpp"

Engine::Engine() {
    wakeup_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (wakeup_fd == -1) {
        throw std::runtime_error("Failed to create eventfd");
    }

    reactor = std::make_unique<Reactor>(wakeup_fd);

    wakeup_handler = std::make_unique<WakeupHandler>(wakeup_fd, reactor.get());
    reactor->set_wakeup_handler(wakeup_handler.get());
    reactor->add_handler(wakeup_handler.get());
}

Engine::~Engine() {
    close(wakeup_fd);
}

int Engine::setup_listening_fd(const bool blocking) {
    int fd = socket(AF_INET, SOCK_STREAM | (blocking ? 0 : SOCK_NONBLOCK), 0);
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    return fd;
}

void Engine::bind_to_port(const int port, const int server_fd) {
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, SOMAXCONN);
}

void Engine::register_acceptor(EventHandler* acceptor) {
    reactor->add_handler(acceptor);
}

void Engine::start() {
    reactor->start();
}

Reactor* Engine::get_reactor() const {
    return reactor.get();
}