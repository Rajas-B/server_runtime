#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <memory>
#include <functional>
#include <vector>

class Reactor;
class EventHandler;
class WakeupHandler;
class Acceptor;
class ProtocolHandler;

class Server { 
public:
    Server();
    ~Server();

    void start();
    void register_acceptor(EventHandler* acceptor);
    // implicitly TCP for now
    void listen_on(int port, std::function<ProtocolHandler*()> protocol_handler_factory);
    Reactor* get_reactor() const;

private:
    int wakeup_fd;
    std::unique_ptr<Reactor> reactor;
    std::unique_ptr<WakeupHandler> wakeup_handler;
    std::vector<std::unique_ptr<Acceptor>> acceptors;
};