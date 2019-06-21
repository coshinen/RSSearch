 ///
 /// @file    EpollPoller.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-04 22:51:08
 ///
 
#include "./net/EpollPoller.h"
#include "./net/SocketUtil.h"
#include "./net/Acceptor.h"

#include <assert.h>

namespace md
{

EpollPoller::EpollPoller(Acceptor & acceptor)
: _acceptor(acceptor)
, _epollfd(createEpollFd())
, _listenfd(_acceptor.fd())
, _eventfd(createEventFd())
, _isLooping(false)
, _mutex()
, _eventsList(1024)
{
    addEpollReadFd(_epollfd, _listenfd);
    addEpollReadFd(_epollfd, _eventfd);
}

EpollPoller::~EpollPoller()
{
    ::close(_epollfd);
}

void EpollPoller::loop()
{
    _isLooping = true;
    ::printf("---- Server start ----\n");
    while (_isLooping)
    {
        waitEpollfd();
    }
}

void EpollPoller::unloop()
{
    if (_isLooping)
        _isLooping = false;
}

void EpollPoller::runInLoop(const Functor & cb)
{
    {
    MutexLockGuard mlg(_mutex);
    _pendingFunctors.push_back(cb);
    }
    wakeup();
}

void EpollPoller::doPendingFunctors()
{
    ::printf("doPendingFunctors()\n");
    std::vector<Functor> functors;
    {
    MutexLockGuard mlg(_mutex);
    functors.swap(_pendingFunctors);
    }
    for (auto & functor : functors)
    {
        functor();
    }
}

void EpollPoller::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(_eventfd, &one, sizeof(one));
    if (n != sizeof(one)) {
        ::perror("EpollPoller::wakeup() n != 8");
    }
}

void EpollPoller::handleRead()
{
    uint64_t howmany;
    ssize_t n = ::read(_eventfd, &howmany, sizeof(howmany));
    if (n != sizeof(howmany)) {
        ::perror("EpollPoller::handleRead() n != 8");
    }
}

void EpollPoller::setConnectionCallback(EpollCallback cb)
{
    _onConnectionCb = cb;
}

void EpollPoller::setMessageCallback(EpollCallback cb)
{
    _onMessageCb = cb;
}

void EpollPoller::setCloseCallback(EpollCallback cb)
{
    _onCloseCb = cb;
}
    
void EpollPoller::waitEpollfd()
{
    ::printf("EpollPoller::waitEpollfd()\n");
    int nready;
    do
    {
        nready = ::epoll_wait(_epollfd,
                    &(*_eventsList.begin()),
                    _eventsList.size(),
                    5000); // 单位毫秒
    } while (-1 == nready && EINTR == errno);

    if (-1 == nready) {
        perror("epoll_wait error");
        exit(EXIT_FAILURE);
    } else if (0 == nready) {
        printf("epoll_wait timeout\n");
    } else {
        //做一个扩容的操作
        if (static_cast<int>(_eventsList.size()) == nready) {
            _eventsList.resize(_eventsList.size() * 2);
        }

        for (int idx = 0; idx != nready; ++idx)
        {
            if (_listenfd == _eventsList[idx].data.fd) {
                if (_eventsList[idx].events & EPOLLIN) {
                    handleConnection();
                }
            } else if(_eventfd == _eventsList[idx].data.fd) {
                ::printf("wakeupfd light\n");
                if (_eventsList[idx].events & EPOLLIN) {
                    handleRead();
                    doPendingFunctors();
                }
            } else {
                if (_eventsList[idx].events & EPOLLIN) {
                    handleMessage(_eventsList[idx].data.fd);
                }
            }
        } // end for
    } // end else
}

void EpollPoller::handleConnection()
{
    int peerfd = _acceptor.accept();
    addEpollReadFd(_epollfd, peerfd);

    TcpConnectionPtr conn(new TcpConnection(peerfd, this));
    //...给客户端发一个欢迎信息==>挖一个空：等...
    //conn->send("welcome to server.\n");
    conn->setConnectionCallback(_onConnectionCb);
    conn->setMessageCallback(_onMessageCb);
    conn->setCloseCallback(_onCloseCb);

    std::pair<ConnectionMap::iterator, bool> ret;
    ret = _connMap.insert(std::make_pair(peerfd, conn));
    assert(true == ret.second);
    //void(ret);
    //_connMap[peerfd] = conn;

    conn->handleConnectionCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
    bool isClosed = isConnectionClosed(peerfd);
    ConnectionMap::iterator it = _connMap.find(peerfd);
    assert(it != _connMap.end());

    if (!isClosed) {
        it->second->handleMessageCallback();
    } else {
        it->second->handleCloseCallback();
        delEpollReadFd(_epollfd, peerfd);
        _connMap.erase(it);
    }
}

} // end of namespace md
