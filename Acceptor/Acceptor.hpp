#include "EventHandler/EventHandler.hpp"
#include "Protocol/IParserFactory.hpp"

namespace reactor {
    class Reactor;
}

class Acceptor: public EventHandler {
public:
    int getfd () {
        return server_fd;
    }
    Acceptor(std::shared_ptr<reactor::Reactor>& reactor, int server_fd);
    void handle_read() override;
    void handle_write() override;   
    void handle_close() override;
private:
    int server_fd;
    std::shared_ptr<reactor::Reactor> reactor;
    IParserFactory* parserFactory;
};