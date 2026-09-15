class EventHandler {
public:
    virtual int getfd() = 0;
    virtual void handle_read() = 0;
    virtual void handle_write() = 0;
    virtual void handle_close() = 0;
};