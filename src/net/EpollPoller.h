 ///
 /// @file    EpollPoller.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 22:26:13
 ///

#ifndef __EPOLLPOLLER_H__
#define __EPOLLPOLLER_H__

#include "Noncopyable.h"
#include "TcpConnection.h"
#include "./bo_threadpool/MutexLock.h"

#include <sys/epoll.h>

#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace md
{

class Acceptor;

class EpollPoller
: Noncopyable
{
public:
    typedef TcpConnection::TcpConnectionCallback EpollCallback;
    typedef std::function<void()> Functor;

    EpollPoller(Acceptor & acceptor);
    ~EpollPoller();

    void loop();
    void unloop();
    void runInLoop(const Functor & cb);
    void doPendingFunctors();

    void wakeup();
    void handleRead();

    void setConnectionCallback(EpollCallback cb);
    void setMessageCallback(EpollCallback cb);
    void setCloseCallback(EpollCallback cb);
private:
    void waitEpollfd();
    void handleConnection();
    void handleMessage(int peerfd);
private:
    Acceptor & _acceptor;
    int _epollfd;
    int _listenfd;
    int _eventfd;
    bool _isLooping;

    MutexLock _mutex;
    std::vector<Functor> _pendingFunctors; // 为什么不先开空间，和_eventsList一样？

    typedef std::vector<struct epoll_event> EventList;
    EventList _eventsList;

    typedef std::map<int, TcpConnectionPtr> ConnectionMap;
    ConnectionMap _connMap;

    EpollCallback _onConnectionCb;
    EpollCallback _onMessageCb;
    EpollCallback _onCloseCb;
};

} // end of namespace md

#endif
