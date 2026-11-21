#pragma once

#include "ConnectionContext.hpp"

class TcpConnection;

class TcpConnectionContext: public ConnectionContext {
public:
    TcpConnectionContext(TcpConnection* conn_);
    void pause_reading() override;
    void resume_reading() override;
private:
    TcpConnection* conn;
};