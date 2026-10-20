#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <memory>

class Reactor;
class EventHandler;
class WakeupHandler;

class Server { 
public:
    Server();
    ~Server();
    void register_acceptor(EventHandler* acceptor);
    void start();
    
    Reactor* get_reactor() const;

private:
    int wakeup_fd;
    std::unique_ptr<Reactor> reactor;
    std::unique_ptr<WakeupHandler> wakeup_handler;
};