#include "MessageQueue.hpp"
#include "EventContext.hpp"

void MessageQueue::add_to_write_queue(uint64_t conn_id, std::vector<uint8_t>* message) {
    {
        std::lock_guard<std::mutex> lock(queue_guard);
        // make pair and push onto queue
        messages_to_write.push(std::make_pair(conn_id, message));
    }
    notify_reactor();
}

void MessageQueue::notify_reactor() {
    reactor_context->request_write_wakeup();
}

void MessageQueue::transfer_messages_to_write(std::queue<std::pair<uint64_t, std::vector<uint8_t>*>>& current_messages) {
    std::lock_guard<std::mutex> lock(queue_guard);
    std::swap(messages_to_write, current_messages);
}