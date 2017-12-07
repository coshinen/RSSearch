 ///
 /// @file    Thread.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-24 11:18:42
 ///

#ifndef __THREAD_H__
#define __THREAD_H__

#include "Noncopyable.h"

#include <pthread.h>

#include <functional>

namespace my
{

class Thread
: Noncopyable
{
	typedef std::function<void()> ThreadCallback;
public:
	Thread(ThreadCallback && cb);
	~Thread();

	void start();
	void join();
	bool isRunning() const;

	::pthread_t getPthId();
private:
	static void * threadFunc(void * arg);
private:
	::pthread_t _pthId;
	bool _isRunning;

	ThreadCallback _cb;
};

} // end of namespace my

#endif
