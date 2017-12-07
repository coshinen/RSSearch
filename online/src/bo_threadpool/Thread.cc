 ///
 /// @file    Thread.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-24 11:24:26
 ///

#include "./bo_threadpool/Thread.h"

#include <stdio.h>

#include <utility>

namespace my
{

Thread::Thread(ThreadCallback && cb)
: _pthId(0)
, _isRunning(false)
, _cb(std::move(cb))
{}

Thread::~Thread()
{
	::printf("~Thread()\n");
	if (_isRunning) {
		if(-1 == ::pthread_detach(_pthId)) {
			::perror("pthread_detach");
		} else {
			::printf("detach succeeded\n");
		}
		_isRunning = false;
	}
}

void Thread::start()
{
	::pthread_create(&_pthId, NULL, threadFunc, this);
	_isRunning = true;
}

void Thread::join()
{
	if (_isRunning) {
		::pthread_join(_pthId, NULL);
		_isRunning = false;
	}
}

bool Thread::isRunning() const
{ return _isRunning; }

::pthread_t Thread::getPthId()
{ return _pthId; }

void * Thread::threadFunc(void * arg)
{
	Thread * pThread = static_cast<Thread*>(arg);
	if (pThread)
		pThread->_cb();
	return NULL;
}

} // end of namespace my
