 ///
 /// @file    tcpconnection.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 17:04:23
 ///
 
#include "tcpconnection.h"
#include "epollpoller.h"

#include <stdio.h>
#include <string.h>

namespace md
{

TcpConnection::TcpConnection(int sockfd, EpollPoller * poller)
: _sockfd(sockfd)
, _sockIO(sockfd)
, _localAddr(Socket::getLocalAddr(sockfd))
, _peerAddr(Socket::getPeerAddr(sockfd))
, _isShutdownWrite(false)
, _poller(poller)
{
    _sockfd.nonblock();
}

TcpConnection::~TcpConnection()
{
    if (!_isShutdownWrite) {
        shutdown();
    }
    ::printf("~TcpConnection()\n");
}

std::string TcpConnection::receive()
{
    char buf[65536]; // 64K
    ::bzero(buf, sizeof(buf));
    size_t ret = _sockIO.readLine(buf, sizeof(buf));
    if (0 == ret) {
        return std::string();
    } else
        return std::string(buf);
}

void TcpConnection::send(const std::string & msg)
{
    _sockIO.writeN(msg.c_str(), msg.size());
}
//它是在计算线程里调用的。
void TcpConnection::sendInLoop(const std::string & msg)
{
    _poller->runInLoop(std::bind(&TcpConnection::send, this, msg));
}

void TcpConnection::shutdown()
{
    if (!_isShutdownWrite) {
        _sockfd.shutdownWrite();
        _isShutdownWrite = true;
    }
}

std::string TcpConnection::toString()
{
    char str[100];
    snprintf(str, sizeof(str), "%s: %d -> %s: %d",
            _localAddr.ip().c_str(),
            _localAddr.port(),
            _peerAddr.ip().c_str(),
            _peerAddr.port());
    return std::string(str);

}

void TcpConnection::setConnectionCallback(TcpConnectionCallback cb)
{ _onConnectionCb = cb; }

void TcpConnection::setMessageCallback(TcpConnectionCallback cb)
{ _onMessageCb = cb; }

void TcpConnection::setCloseCallback(TcpConnectionCallback cb)
{ _onCloseCb = cb; }

void TcpConnection::handleConnectionCallback()
{
    if (_onConnectionCb)
        _onConnectionCb(shared_from_this());
}

void TcpConnection::handleMessageCallback()
{
    if (_onMessageCb)
        _onMessageCb(shared_from_this());
}

void TcpConnection::handleCloseCallback()
{
    if (_onCloseCb)
        _onCloseCb(shared_from_this());
}

} // end of namespace md
