 ///
 /// @file    MutexLock.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-10-31 19:19:01
 ///

#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include "Noncopyable.h"

#include <pthread.h>

namespace md
{

class MutexLock
: Noncopyable
{
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unlock();

    ::pthread_mutex_t * getMutexPtr();
    bool isLocking() const;
private:
    ::pthread_mutex_t _mutex;
    bool _isLocking;
};

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock & mutex)
    : _mutex(mutex)
    {
        _mutex.lock();
    }
    ~MutexLockGuard()
    {
        _mutex.unlock();
    }
private:
    MutexLock & _mutex;
};

} // end of namespace md

#endif
