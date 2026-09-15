#pragma once
#include "EventHandler/EventHandler.hpp"
#include <stdint.h>

class ClientHandler: public EventHandler {
public:
    int getfd();
    void handle_read();
    void handle_write();
    void handle_close();
    ClientHandler(int clientfd);
private:
    int fd;
    uint8_t buf[8*1024];
};