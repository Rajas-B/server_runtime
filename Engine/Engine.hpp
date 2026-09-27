#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <memory>

#include "Reactor/Reactor.hpp"
#include "WakeupHandler/WakeupHandler.hpp"
#include "Acceptor/Acceptor.hpp"

class EventHandler; 

class Engine { 
public:
    Engine();
    ~Engine();
    void register_acceptor(EventHandler* acceptor);
    void start();
    
    Reactor* get_reactor() const;

private:
    int wakeup_fd;
    std::unique_ptr<Reactor> reactor;
    std::unique_ptr<WakeupHandler> wakeup_handler;
};