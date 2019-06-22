 ///
 /// @file    tcpserver.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-05 12:25:03
 ///

#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "acceptor.h"
#include "epollpoller.h"

#include <string>

namespace md
{

class TcpServer
{
    typedef EpollPoller::EpollCallback TcpServerCallback;
public:
    TcpServer(unsigned short port);
    TcpServer(const std::string & ip, unsigned short port);

    void start();
    void stop();

    void setConnectionCallback(TcpServerCallback cb);
    void setMessageCallback(TcpServerCallback cb);
    void setCloseCallback(TcpServerCallback cb);
private:
    Acceptor _acceptor;
    EpollPoller _poller;

    TcpServerCallback _connectionCb;
    TcpServerCallback _messageCb;
    TcpServerCallback _closeCb;
};

} // end of namespace md

#endif
