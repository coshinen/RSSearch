 ///
 /// @file    Acceptor.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 15:25:43
 ///
 
#include "Acceptor.h"
#include "SocketUtil.h"

namespace md
{

Acceptor::Acceptor(int listenfd, const InetAddress & addr)
: _listenSock(listenfd)
, _addr(addr)
{}

void Acceptor::ready()
{
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}

int Acceptor::accept()
{
    int peerfd = ::accept(_listenSock.fd(), NULL, NULL);
    if (-1 == peerfd) {
        perror("accept error");
    }
    return peerfd;
}
    
void Acceptor::setReuseAddr(bool on)
{
    int flag = (on ? 1 : 0);
    if (-1 == ::setsockopt(_listenSock.fd(),
                SOL_SOCKET,
                SO_REUSEADDR,
                &flag,
                static_cast<socklen_t>(sizeof(flag)))) {
        ::perror("setsockopt reuseaddr error");
        ::close(_listenSock.fd());
        ::exit(EXIT_FAILURE);
    }
}

void Acceptor::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
    int flag = (on ? 1 : 0);
    if (-1 == ::setsockopt(_listenSock.fd(),
                SOL_SOCKET,
                SO_REUSEPORT,
                &flag,
                static_cast<socklen_t>(sizeof(flag)))) {
        ::perror("setsockopt reuseport error");
        ::close(_listenSock.fd());
        ::exit(EXIT_FAILURE);
    }
#else
    if (on)
        ::fprintf(stderr, "SO_REUSEPORT is not supported!\n");
#endif
}

void Acceptor::bind()
{
    if (-1 == ::bind(_listenSock.fd(),
                (const struct sockaddr *)_addr.getSockAddrPtr(),
                sizeof(struct sockaddr_in))) {
        ::perror("bind error");
        ::close(_listenSock.fd());
        ::exit(EXIT_FAILURE);
    }
}

void Acceptor::listen()
{
    if (-1 == ::listen(_listenSock.fd(), 10)) {
        ::perror("listen error");
        ::close(_listenSock.fd());
        ::exit(EXIT_FAILURE);
    }
}

} // end of namespace md
