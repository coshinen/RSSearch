 ///
 /// @file    TcpServer.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-05 12:32:17
 ///

#include "./net/TcpServer.h"
#include "./net/SocketUtil.h"
#include "./net/InetAddress.h"

namespace my
{

TcpServer::TcpServer(const std::string & ip, unsigned short port)
: _acceptor(createSocketFd(), InetAddress(ip.c_str(), port))
, _poller(_acceptor)
{}

void TcpServer::start()
{
	_acceptor.ready();
	_poller.setConnectionCallback(_connectionCb);
	_poller.setMessageCallback(_messageCb);
	_poller.setCloseCallback(_closeCb);
	_poller.loop();
}

void TcpServer::stop()
{
	_poller.unloop();
}

void TcpServer::setConnectionCallback(TcpServerCallback cb)
{ _connectionCb = cb; }

void TcpServer::setMessageCallback(TcpServerCallback cb)
{ _messageCb = cb; }

void TcpServer::setCloseCallback(TcpServerCallback cb)
{ _closeCb = cb; }

} // end of namespace my
