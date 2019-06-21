 ///
 /// @file    Condition.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-10-31 19:32:36
 ///

#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "Noncopyable.h"

#include <pthread.h>

namespace md
{

class MutexLock;

class Condition
: Noncopyable
{
public:
    Condition(MutexLock & mutex);
    ~Condition();

    void wait();
    void notify();
    void notifyAll();
private:
    pthread_cond_t _cond;
    MutexLock & _mutex;
};

} // end of namespace md

#endif
