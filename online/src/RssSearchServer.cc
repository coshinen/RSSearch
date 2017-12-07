 ///
 /// @file    RssSearchServer.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-08 07:54:20
 ///

#include "RssSearchServer.h"
#include "Configuration.h"
#include "./net/SocketUtil.h"
#include "./cache/CacheManager.h"
#include "./timer/TimerThread.h"

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
	_threadpool.start();

	CacheManager::initCache(_threadpool.getPthIds(), Configuration::getInstance()->getConfigMap()[CACHE_PATH].c_str());
	
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
	conn->sendInLoop(_rssSearch.doQuery(msg));
}

} // end of namespace my
