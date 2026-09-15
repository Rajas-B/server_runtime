#include "ClientHandler.hpp"

#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>

ClientHandler::ClientHandler(int fd): fd(fd) {}

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
    }
}
