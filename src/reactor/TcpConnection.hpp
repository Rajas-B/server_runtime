#pragma once
#include "Connection.hpp"
#include "ConnectionHandler.hpp"

#include <mutex>
#include <memory>
#include <queue>
#include <atomic>
#include <vector>
#include <stdint.h>

class ProtocolHandler;
class Reactor;

class TcpConnection: public ConnectionHandler {
public:
    // EventHandler traits
    int getfd() override;
    void handle_read() override;
    void handle_write() override;
    void handle_close() override;

    // ConnectionHandler traits
    std::chrono::steady_clock::time_point get_last_active_timestamp() override;
    // int get_ttl() override;
    // void handle_timeout() override;
    uint64_t get_conn_id() override;
    void on_connection_established() override; // add timeouts and other related tasks
    void graceful_shutdown() override;
    void process_message(std::vector<uint8_t>* message);
    void send_message(std::vector<uint8_t>* message) override;
    void pause_reading() override;
    void resume_reading() override;
private:
    void update_events_to_monitor(int events_to_update);
    int current_events_to_monitor = 0;
    int fd;
    uint64_t conn_id;
    uint8_t buf[8*1024];
    std::vector<uint8_t>* current_message_to_send;
    std::unique_ptr<ProtocolHandler> protocol_handler;
    std::mutex write_guard;
    std::deque<std::vector<uint8_t>> write_queue;
    std::atomic<bool> is_being_processed{false}; // set to true when this handler is added in reactor's queue, to avoid duplicate entries
    Reactor* reactor;
};