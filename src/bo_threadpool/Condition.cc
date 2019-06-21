 ///
 /// @file    Condition.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-02 15:30:03
 ///

#include "./bo_threadpool/Condition.h"
#include "./bo_threadpool/MutexLock.h"

namespace md
{

Condition::Condition(MutexLock & mutex)
: _mutex(mutex)
{
    pthread_cond_init(&_cond, NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&_cond);
}

void Condition::wait()
{
    pthread_cond_wait(&_cond, _mutex.getMutexPtr());
}

void Condition::notify()
{
    pthread_cond_signal(&_cond);
}

void Condition::notifyAll()
{
    pthread_cond_broadcast(&_cond);
}

} // end of namespace md
