#include <sys/epoll.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <memory>
#include <chrono>

#include "Reactor.hpp"
#include "WakeupHandler.hpp"
#include "TcpConnection.hpp"
#include "MessageQueue.hpp"


int Reactor::start() {
    // the infinite loop
    struct epoll_event events[64]; // the events (which will be new connection or a read event)

    while (true) {
        
        int timeout = -1;
        if (!timer_mngr->timers.empty()) {
            timeout = timer_mngr->ms_until_earliest_timer();
        }
        // block till atleast one socket has an event;
        int num_events = epoll_wait(epoll_fd, events, 64, timeout); // no timeout
        
        if (num_events == 0) {
            timer_mngr->execute_expired_timer();
        }

        for (int i = 0; i < num_events; ++i) {
            EventHandler* handler = static_cast<EventHandler*>(events[i].data.ptr);
            if (events[i].events & EPOLLIN) {
                handler->handle_read();
            }
            if (events[i].events & EPOLLOUT) {
                handler->handle_write();
            }
            // only for errosr, other graceful shutdowns need to be handled elsewhere
            if (events[i].events & EPOLLERR) {
                handler->handle_close();
            }

        }
    }
    return 0;
}

Reactor::Reactor(){
    epoll_fd = epoll_create1(0);
    int wakeup_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (wakeup_fd == -1) {
        throw std::runtime_error("Failed to create eventfd");
    }
    wakeup_handler = std::make_unique<WakeupHandler>(wakeup_fd, this);
    add_handler(wakeup_handler.get());
    message_queue = std::make_unique<MessageQueue>();
}


void Reactor::add_handler(EventHandler* handler) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = handler;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, handler->getfd(), &ev);
}

void Reactor::add_to_write_ready(TcpConnection* handler) {
    {
        std::lock_guard<std::mutex> lock(write_guard);
        ready_to_write_clients.push(handler);
    }

    // wakeup signal
    wakeup_reactor();
}

void Reactor::wakeup_reactor() {
    wakeup_handler->handle_write();
}

// called by wakeup handler
void Reactor::process_write_clients() {
    std::queue<std::pair<uint64_t, std::vector<uint8_t>*>> current_messages;
    message_queue->transfer_messages_to_write(current_messages);
    while (!current_messages.empty()) {
        auto& it = current_messages.front();
        TcpConnection* connection = get_handler_if_alive(it.first);
        if (!connection) { // connection is dead
            current_messages.pop();
            continue;
        }
        connection->process_message(it.second); // vector is still alive
        current_messages.pop();
    }
}

int Reactor::get_epoll_fd() {
    return epoll_fd;
}

void Reactor::modify_epoll(int events, EventHandler* handler) {
    struct epoll_event ev;
    ev.data.ptr = handler;
    ev.events = events;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, handler->getfd(), &ev);
}

uint64_t Reactor::generate_id() {
    return next_connection_id.fetch_add(1, std::memory_order_relaxed);
}

void Reactor::register_handler(uint64_t id, TcpConnection* handler) {
    active_handlers[id] = handler;
}

void Reactor::unregister_handler(uint64_t id) {
    active_handlers.erase(id);
}

TcpConnection* Reactor::get_handler_if_alive(uint64_t id) {
    auto it = active_handlers.find(id);
    if (it != active_handlers.end()) {
        return it->second;
    }
    return nullptr;
}

void Reactor::register_timer(int expiration, std::function<void()> task) {
    timer_mngr->add_timer(expiration, task);
}