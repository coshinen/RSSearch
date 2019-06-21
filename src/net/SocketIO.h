 ///
 /// @file    SocketIO.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 16:06:03
 ///

#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

#include <stdio.h>

namespace my
{

class SocketIO
{
public:
    SocketIO(int sockfd);

    size_t readN(char * buf, size_t count);
    size_t writeN(const char * buf, size_t count);
    size_t readLine(char * buf, size_t maxLen);
private:
    size_t recv_peek(char * buf, size_t count);
private:
    int _sockfd;
};

} // end of namespace my

#endif
