#include "Engine.hpp"
#include "Reactor.hpp"
#include "WakeupHandler.hpp"

Engine::Engine() {
    reactor = std::make_unique<Reactor>();
}

Engine::~Engine() {
    close(wakeup_fd);
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