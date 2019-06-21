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

namespace curthread
{

__thread const char * threadName = "my";

} // end of namespace curthread

ThreadData::ThreadData(ThreadCallback && cb, const std::string & name)
: _cb(std::move(cb))
, _name(name)
{}

void ThreadData::runInThread()
{
    curthread::threadName = _name.empty() ? "my" : _name.c_str();
    ::printf("I am thread: %s\n", curthread::threadName);

    if (_cb)
        _cb();
}

Thread::Thread(ThreadCallback && cb, const std::string & name)
: _pthId(0)
, _isRunning(false)
, _cb(std::move(cb))
, _name(name)
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
    ThreadData * pThreadData = new ThreadData(std::move(_cb), _name);
    ::pthread_create(&_pthId, NULL, threadFunc, pThreadData);
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
    ThreadData * pThreadData = static_cast<ThreadData*>(arg);
    if (pThreadData)
        pThreadData->runInThread();
    return NULL;
}

} // end of namespace my
