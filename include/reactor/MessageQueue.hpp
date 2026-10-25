#pragma once

#include <queue>
#include <stdint.h>
#include <vector>
#include <atomic>
#include <mutex>

class EventContext;
// this is a common queue across the reactor loop
// queue consists of a pair<uint64_t, std::vector<uint8_t>*> {conn_id, message}
class MessageQueue {
public:
    void add_to_write_queue(uint64_t conn_id, std::vector<uint8_t>* message);
    ~MessageQueue();
    void transfer_messages_to_write(std::queue<std::pair<uint64_t, std::vector<uint8_t>*>>& current_messages);
private:
    std::queue<std::pair<uint64_t, std::vector<uint8_t>*>> messages_to_write;
    void notify_reactor(); // notify reactor thread that there is a message to be written
    std::atomic<bool> is_being_processed = false; // flag to not wake up reactor again and again
    std::mutex queue_guard; // queue is accessed by both reactor and application threads
    EventContext* reactor_context;
};