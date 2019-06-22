 ///
 /// @file    socketio.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 16:11:43
 ///
 
#include "socketio.h"
#include "socketutil.h"

namespace md
{

SocketIO::SocketIO(int sockfd)
: _sockfd(sockfd)
{}

size_t SocketIO::readN(char * buf, size_t count)
{
    size_t nleft = count;
    char * pbuf = buf;
    while (nleft > 0)
    {
        int nread = ::read(_sockfd, pbuf, nleft);
        if (-1 == nread) {
            if (EINTR == errno) // 忽略掉中断信号
                continue;
            return EXIT_FAILURE;
        } else if (0 == nread) { // EOF
            break;
        }
        pbuf += nread;
        nleft -= nread;
    }
    return (count - nleft);
}

size_t SocketIO::writeN(const char * buf, size_t count)
{
    size_t nleft = count;
    const char * pbuf = buf;
    while (nleft > 0)
    {
        int nwrite = ::write(_sockfd, pbuf, nleft);
        if (-1 == nwrite) { // EINTR阻塞操作被信号中断返回的错误码。
            if (EINTR == errno)
                continue;
            return EXIT_FAILURE;
        }
        pbuf += nwrite;
        nleft -= nwrite;
    }
    return (count - nleft);
}

size_t SocketIO::readLine(char * buf, size_t maxLen)
{
    size_t nleft = maxLen - 1;
    char * pbuf = buf;
    size_t total = 0;
    while (nleft > 0)
    {
        int nread = recv_peek(pbuf, nleft);
        if (nread <= 0)
            return nread;
        //检查'\n'
        for (size_t idx = 0; idx != (size_t)nread; ++idx)
        {
            if ('\n' == pbuf[idx]) {
                size_t nsize = idx + 1;
                if (readN(pbuf, nsize) != nsize)
                    return EXIT_FAILURE;
                pbuf += nsize;
                *pbuf = 0;
                total += nsize;
                return total;
            }
        }
        if (readN(pbuf, (size_t)nread) != (size_t)nread)
            return EXIT_FAILURE;
        nleft -= nread;
        total += nread;
        pbuf += nread;
    }
    *pbuf = 0;
    return maxLen - 1;
}

size_t SocketIO::recv_peek(char * buf, size_t count)
{
    int nread;
    do
    { // MSG_PEEK标志将套接字对应内核读缓冲区的数据拷贝用户缓冲区，而不会使内核缓冲区的数据减少。
        nread = ::recv(_sockfd, buf, count, MSG_PEEK);
    } while (-1 == nread && EINTR == errno);
    return nread;
}

} // end of namespace md
