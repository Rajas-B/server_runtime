#pragma once

#include <sys/socket.h>
#include <sys/eventfd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <memory>

#include "Reactor/Reactor.hpp"
#include "WakeupHandler/WakeupHandler.hpp"

class EventHandler; 

class Engine { 
public:
    Engine();
    ~Engine();
    static int setup_listening_fd(const bool blocking = false);
    static void bind_to_port(const int port, const int server_fd);
    void register_acceptor(EventHandler* acceptor);
    void start();
    
    Reactor* get_reactor() const;

private:
    int wakeup_fd;
    std::unique_ptr<Reactor> reactor;
    std::unique_ptr<WakeupHandler> wakeup_handler;
};