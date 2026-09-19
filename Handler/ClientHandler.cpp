#include "ClientHandler.hpp"

#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>

ClientHandler::ClientHandler(int fd, HTTPParser* parser, int wakeup_fd, reactor::Reactor*& reactor): 
fd(fd), parser(parser), wakeup_fd(wakeup_fd), reactor(reactor) {}

int ClientHandler::getfd() {
    return fd;
}

void ClientHandler::handle_read() {
    while (true) {
        ssize_t bytes_read = read(fd, buf, 8192);
        if (bytes_read == -1 && errno == EAGAIN){    
            break;
        }
        if (bytes_read == 0) {
            handle_close();
            break;
        }
        // parser consume
        if (parser->consume(buf, bytes_read) == Status::ERROR) {
            break;
        }
    }
}

// will be called once the response is completed
void ClientHandler::enqueue_response_and_wake(std::vector<uint8_t> response_bytes) {
    // thread safe to push response bytes onto queue, to keep responses continuguous
    {
        std::lock_guard<std::mutex> lock(write_mutex);
        write_queue.push_back(std::move(response_bytes));
    }
    // create a wakeup signal
    // this will unlock epoll wait because there is an event on the wakeup handler
    if (is_being_processed.exchange(true) == false) {
        reactor->add_to_write_ready(this);
    }
}

void ClientHandler::handle_write() {
    // we have a queue of messages to write
    // write till the queue is empty
    std::lock_guard<std::mutex> lock(write_guard);

    while(!write_queue.empty()) {
        // take the message at the top
        auto& message = write_queue.top();
        ssize_t bytes_written = write(fd, message.data(), message.size());

        if (bytes_written == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            // the OS buffer is full, can't write now
            break;
        }
        if (bytes_written < message.size()) {
            // partial write occurred
            // update the first message to contain only the remaining bytes
            message.erase(message.begin(), message.begin() + bytes_written);
            // break now since a partial write indicates that the OS buffer might be full
            break;
        }
        else {
            write_queue.pop_front();
        }
    }
}

bool ClientHandler::ready_for_write() {
    return !write_queue.empty();
}

// not in the reactor's queue anymore, called in reactor
void ClientHandler::mark_as_processed() {
    is_being_processed.store(false);
}
