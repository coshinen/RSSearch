 ///
 /// @file    inetaddress.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 15:00:22
 ///

#include "inetaddress.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace md
{

InetAddress::InetAddress(unsigned short port)
{
    ::bzero(&_addr, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = ::htons(port);
    _addr.sin_addr.s_addr = INADDR_ANY;
}

InetAddress::InetAddress(const char * pIp, unsigned short port)
{
    ::bzero(&_addr, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = ::htons(port);
    _addr.sin_addr.s_addr = ::inet_addr(pIp);
}

InetAddress::InetAddress(const struct sockaddr_in & addr)
: _addr(addr)
{}

std::string InetAddress::ip() const
{
    return std::string(::inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const
{
    return ::ntohs(_addr.sin_port);
}

const struct sockaddr_in * InetAddress::getSockAddrPtr() const
{
    return &_addr;
}

} // end of namespace md
