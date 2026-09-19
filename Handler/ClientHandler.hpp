#pragma once
#include "EventHandler/EventHandler.hpp"
#include "HTTP/v1.1/HTTPParser.hpp"
#include "Reactor/Reactor.hpp"

class ClientHandler: public EventHandler {
public:
    int getfd();
    void handle_read();
    void handle_write();
    void handle_close();
    void enqueue_response_and_wake(std::vector<uint8_t> response_bytes);
    ClientHandler(int clientfd, HTTPParser* parser, int wakeup_fd, reactor::Reactor*& reactor);
    bool ready_for_write() override;
    void mark_as_processed();
private:
    int fd;
    int wakeup_fd;
    uint8_t buf[8*1024];
    HTTPParser* parser;
    std::mutex write_mutex;
    std::deque<std::vector<uint8_t>> write_queue;
    std::atomic<bool> is_being_processed{false}; // set to true when this handler is added in reactor's queue, to avoid duplicate entries

    reactor::Reactor* reactor;
};