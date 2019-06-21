 ///
 /// @file    Acceptor.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 15:20:16
 ///

#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "Socket.h"
#include "InetAddress.h"

namespace my
{

class Acceptor
{
public:
    Acceptor(int listenfd, const InetAddress & add);

    void ready();
    int accept();

    int fd() const { return _listenSock.fd(); }
private:
    void setReuseAddr(bool on);
    void setReusePort(bool on);

    void bind();
    void listen();
private:
    Socket _listenSock;
    InetAddress _addr;
};

} // end of namespace my

#endif
