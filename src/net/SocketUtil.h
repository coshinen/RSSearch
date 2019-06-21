 ///
 /// @file    SocketUtil.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 14:34:14
 ///

#ifndef __SOCKETUTIL_H__
#define __SOCKETUTIL_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#include <string>
#include <sstream>

namespace my
{

inline int createSocketFd()
{
    int sfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sfd) {
        ::perror("socket create error!");
    }
    return sfd;
}

inline void setNonblock(int fd)
{
    int flags = ::fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    ::fcntl(fd, F_SETFL, flags);
}

inline int createEpollFd()
{
    int efd = ::epoll_create1(0);
    if (-1 == efd) {
        perror("epoll_create1 error");
        exit(EXIT_FAILURE);
    }
    return efd;
}

inline int createEventFd()
{
    int eventfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (-1 == eventfd) {
        perror("eventfd create error");
        exit(EXIT_FAILURE);
    }
    return eventfd;
}

inline void addEpollReadFd(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
    if (-1 == ret) {
        perror("epoll_ctl add error");
        exit(EXIT_FAILURE);
    }
}

inline void delEpollReadFd(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    int ret = epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev);
    if (-1 == ret) {
        perror("epoll_ctl del error");
        exit(EXIT_FAILURE);
    }
}

//预览数据
inline size_t recvPeek(int sockfd, void * buf, size_t len)
{
    int nread;
    do
    {
        nread = ::recv(sockfd, buf, len, MSG_PEEK);
    } while (-1 == nread && EINTR == errno);
    return nread;
}

//通过预览数据，判断conn是否关闭
inline bool isConnectionClosed(int sockfd)
{
    char buf[1024];
    int nread = recvPeek(sockfd, buf, sizeof(buf));
    if (-1 == nread) {
        perror("recvPeek");
        return true;
    }
    return (0 == nread);
}

inline int str2int(const std::string & str)
{
    std::istringstream iss(str);
    int number;
    iss >> number;
    return number;
}

inline unsigned short str2unshort(const std::string & str)
{
    std::istringstream iss(str);
    unsigned short number;
    iss >> number;
    return number;
}

} // end of namespace my

#endif
