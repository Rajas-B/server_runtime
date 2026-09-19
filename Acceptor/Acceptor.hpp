#pragma once

#include "EventHandler/EventHandler.hpp"

class IParserFactory;
namespace reactor {
    class Reactor;
}

class Acceptor: public EventHandler {
public:
    int getfd () {
        return server_fd;
    }
    Acceptor(int server_fd, reactor::Reactor* reactor, int wakeup_fd);
    void handle_read() override;
    void handle_write() override;   
    void handle_close() override;
private:
    int server_fd;
    int wakeup_fd;
    reactor::Reactor* reactor;
    IParserFactory* parserFactory;
};