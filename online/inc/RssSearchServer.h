 ///
 /// @file    RssSearchServer.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-08 07:34:26
 ///

#ifndef __RSSSEARCHSERVER_H__
#define __RSSSEARCHSERVER_H__

#include "./net/TcpServer.h"
#include "./bo_threadpool/Threadpool.h"
#include "RssSearch.h"

namespace my
{

class RssSearchServer
{
public:
	RssSearchServer();
	~RssSearchServer();

	void start();
	void stop();
private:
	void onConnection(const TcpConnectionPtr & conn);
	void onMessage(const TcpConnectionPtr & conn);
	void onClose(const TcpConnectionPtr & conn);
	void doTask(const TcpConnectionPtr & conn, const std::string & msg);
private:
	Threadpool _threadpool;
	TcpServer _tcpServer;
	RssSearch _rssSearch;
	bool _isExit;
};

} // end of namespace my

#endif
