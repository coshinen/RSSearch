 ///
 /// @file    Socket.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 14:31:08
 ///

#include "Socket.h"
#include "SocketUtil.h"
#include "InetAddress.h"

namespace my
{

Socket::Socket()
: _sockfd(createSocketFd())
{}

Socket::Socket(int sockfd)
: _sockfd(sockfd)
{}

Socket::~Socket()
{
	::close(_sockfd);
}

void Socket::shutdownWrite()
{
	if (-1 == ::shutdown(_sockfd, SHUT_WR)) {
		perror("shutdown write error!");
	}
}

void Socket::nonblock()
{
	setNonblock(_sockfd);
}

InetAddress Socket::getLocalAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	if (-1 == ::getsockname(sockfd, (struct sockaddr *)&addr, &len)) {
		perror("getsockname error");
	}
	return InetAddress(addr);
}

InetAddress Socket::getPeerAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	if (-1 == ::getpeername(sockfd, (struct sockaddr *)&addr, &len)) {
		perror("getpeername error");
	}
	return InetAddress(addr);
}

} // end of namespace my
