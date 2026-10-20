#include <unistd.h>
#include <stdint.h>

class Connection {
public:
    virtual ~Connection() = default;
    
    virtual ssize_t read_from_connection(uint8_t* buf, size_t max_length) = 0;
    virtual ssize_t write_to_connection(const uint8_t* buf, size_t message_length) = 0;
    
    virtual void close_connection() = 0;
    
    virtual uint64_t get_conn_id() const = 0; 
};