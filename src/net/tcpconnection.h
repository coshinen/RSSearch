 ///
 /// @file    tcpconnection.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 16:58:05
 ///

#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "noncopyable.h"
#include "socket.h"
#include "socketio.h"
#include "inetaddress.h"

#include <memory>
#include <functional>

namespace md
{

class EpollPoller;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection
: Noncopyable
, public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::function<void(const TcpConnectionPtr &)> TcpConnectionCallback;

    TcpConnection(int sockfd, EpollPoller * poller);
    ~TcpConnection();

    std::string receive();
    void send(const std::string & msg);
    void sendInLoop(const std::string & msg);
    void shutdown();

    std::string toString();

    void setConnectionCallback(TcpConnectionCallback cb);
    void setMessageCallback(TcpConnectionCallback cb);
    void setCloseCallback(TcpConnectionCallback cb);

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
private:
    Socket _sockfd;
    SocketIO _sockIO;
    const InetAddress _localAddr;
    const InetAddress _peerAddr;
    bool _isShutdownWrite;
    EpollPoller * _poller;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;
};

} // end of namespace md

#endif
