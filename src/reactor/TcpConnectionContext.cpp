#include "TcpConnectionContext.hpp"

#include "TcpConnection.hpp"

TcpConnectionContext::TcpConnectionContext(TcpConnection* conn_): 
    conn(conn_) {}

void TcpConnectionContext::pause_reading() {
    // pause reading
    conn->pause_reading();
}

void TcpConnectionContext::resume_reading() {
    // resume reading
    conn->resume_reading();
}