 ///
 /// @file    Socket.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 12:31:23
 ///

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "Noncopyable.h"

namespace md
{

class InetAddress;

class Socket
: Noncopyable
{
public:
    Socket();
    Socket(int sockfd);
    ~Socket();

    void shutdownWrite();
    int fd() const { return _sockfd; }

    void nonblock();

    static InetAddress getLocalAddr(int sockfd);
    static InetAddress getPeerAddr(int sockfd);
private:
    int _sockfd;
};

} // end of namespace md

#endif
