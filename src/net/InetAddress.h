 ///
 /// @file    InetAddress.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 14:53:09
 ///

#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <netinet/in.h>
#include <string>

namespace md
{

class InetAddress
{
public:
    InetAddress(unsigned short port);
    InetAddress(const char * pIp, unsigned short port);
    InetAddress(const struct sockaddr_in & addr);

    std::string ip() const;
    unsigned short port() const;
    const struct sockaddr_in * getSockAddrPtr() const;
private:
    struct sockaddr_in _addr;
};

} // end of namespace md

#endif
