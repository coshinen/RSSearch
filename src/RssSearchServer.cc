 ///
 /// @file    RssSearchServer.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-08 07:54:20
 ///

#include "RssSearchServer.h"
#include "Configuration.h"
#include "net/SocketUtil.h"
#include "cache/LRUCache.h"
#include "cache/CacheManager.h"
#include "timer/TimerThread.h"
#include "bo_threadpool/Thread.h"

#include "cpp_redis/cpp_redis"

#include <iostream>
#include <functional>

namespace my
{

RssSearchServer::RssSearchServer()
: _threadpool(str2int(Configuration::getInstance()->getConfigMap()[THREAD_NUM]), str2int(Configuration::getInstance()->getConfigMap()[TASK_NUM]))
, _tcpServer(Configuration::getInstance()->getConfigMap()[IP], str2unshort(Configuration::getInstance()->getConfigMap()[PORT]))
, _rssSearch()
, _isExit(false)
{
    //_rssSearch.showPageLib();
    //_rssSearch.showOffset();
    //_rssSearch.showInvert();
    std::cout << "RssSearchServer()" << std::endl;
}

RssSearchServer::~RssSearchServer()
{
    if (!_isExit)
        stop();
    std::cout << "~RssSearchServer()" << std::endl;
}

void RssSearchServer::start()
{
    /* initialize cache */
    CacheManager::initCache(str2uint(Configuration::getInstance()->getConfigMap()[THREAD_NUM]), Configuration::getInstance()->getConfigMap()[CACHE_PATH].c_str());
    
    _threadpool.start();

    /* start the timer */
    my::TimerThread timerThread(&my::CacheManager::periodicUpdateCaches, 2, 66);
    timerThread.start();

    _tcpServer.setConnectionCallback(std::bind(&RssSearchServer::onConnection, this, std::placeholders::_1));
    _tcpServer.setMessageCallback(std::bind(&RssSearchServer::onMessage, this, std::placeholders::_1));
    _tcpServer.setCloseCallback(std::bind(&RssSearchServer::onClose, this, std::placeholders::_1));

    _tcpServer.start();
}

void RssSearchServer::stop()
{
    if (!_isExit) {
        _tcpServer.stop();
        _threadpool.stop();
        _isExit = true;
    }
}

void RssSearchServer::onConnection(const TcpConnectionPtr & conn)
{
    std::cout << conn->toString() << std::endl;
    conn->send("hello, welcome to Search Engine Server.\n");
}

void RssSearchServer::onMessage(const TcpConnectionPtr & conn)
{
    std::string query(conn->receive());
    query.pop_back();

    //完成了IO线程到计算线程的过渡。
    _threadpool.addTask(std::bind(&RssSearchServer::doTask, this, conn, query));
}

void RssSearchServer::onClose(const TcpConnectionPtr & conn)
{
    std::cout << conn->toString() << " close" << std::endl;
}

void RssSearchServer::doTask(const TcpConnectionPtr & conn, const std::string & msg)
{
    //LRUCache & lruCache = CacheManager::getCache(str2uint(curthread::threadName));
    //if (1 == lruCache.count(msg)) { // cache hit
    //    std::cout << "The result is in the current cache" << std::endl;
    //    conn->sendInLoop(lruCache.getLRUCache(msg));
    //} else { // cache miss
    //    std::cout << "The result is not in the current cache" << std::endl;
    //    conn->sendInLoop(_rssSearch.doQuery(msg));
    //}
    cpp_redis::client client;
    client.connect("192.168.40.128", 6379, [](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
        if (status == cpp_redis::client::connect_state::dropped) {
            std::cout << "client disconnected from " << host << ":" << port << std::endl;
        }
    });

    std::future<cpp_redis::reply> get = client.get(msg);

    // synchronous commit, no timeout
    client.sync_commit();

    cpp_redis::reply reply = get.get();
    if (!reply.is_null()) { // cache hit
        std::cout << "-------> cache hit 22333" << std::endl;
        conn->sendInLoop(reply.as_string());
    } else { // cache miss
        std::cout << "-------- cache miss -_-!" << std::endl;
        conn->sendInLoop(_rssSearch.doQuery(msg));
    }
}

} // end of namespace my
