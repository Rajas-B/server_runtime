#pragma once
#include <sys/epoll.h>
#include <unistd.h>

#include "Reactor.hpp"
#include "EventHandler/EventHandler.hpp"

namespace {

bool set_nonblocking(int& fd, bool nonblocking) {
    if (fd < 0) {
        return false;
    }
    int flags = fcntl(fd, F_GETFL, 0);
    flags = nonblocking ? (flags | O_NONBLOCK): (flags | ~O_NONBLOCK);

    return (fcntl(fd, F_SETFL, flags));
}
}

namespace reactor {
    int Reactor::start() {
        // the infinite loop
        while (true) {
            // block till atleast one socket has an event;
            int num_events = epoll_wait(epoll_fd, events, 64, -1); // no timeout
            
            for (int i = 0; i < num_events; ++i) {
                EventHandler* handler = static_cast<EventHandler*>(events[i].data.ptr);
                if (events[i].events & EPOLLIN) {
                    handler->handle_read();
                }
                if (events[i].events & EPOLLOUT) {
                    handler->handle_write();
                }
                if (events[i].events & EPOLLERR) {
                    handler->handle_close();
                }
            }
        }
        return 0;
    }

    void Reactor::add_handler(EventHandler* handler) {
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = handler->getfd();
        ev.data.ptr = handler;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, handler->getfd(), &ev);
    }
}