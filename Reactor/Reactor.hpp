#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "EventHandler/EventHandler.hpp"

namespace reactor {

class Reactor {
public:
    Reactor(const int& server_fd, const int& port):
 server_fd(server_fd) {
        epoll_fd = epoll_create1(0);
    }
    void add_handler(EventHandler* handler);
    int start();
private:
    // this is the epoll fd
    int epoll_fd;
    int server_fd;
    struct epoll_event events[64]; // the events (which will be new connection or a read event)
};

}
