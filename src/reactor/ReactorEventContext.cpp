#include "ReactorEventContext.hpp"

#include "Reactor.hpp"
#include "ConnectionHandler.hpp"

void ReactorEventContext::bind_handler(ConnectionHandler* client_handler) {
    handler = client_handler;
}

void ReactorEventContext::request_write_wakeup() {
    reactor.modify_epoll(EPOLLIN|EPOLLET|EPOLLOUT, handler);
}

void ReactorEventContext::remove_write_wakeup() {
    reactor.modify_epoll(EPOLLIN|EPOLLET, handler);
}

void ReactorEventContext::add_to_write_queue() {
    reactor.add_to_write_ready(handler);
}

void ReactorEventContext::add_timer(int expiration, std::function<void()> task) {
    reactor.register_timer(expiration, task);
}