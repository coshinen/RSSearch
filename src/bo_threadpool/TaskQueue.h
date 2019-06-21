 ///
 /// @file    TaskQueue.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-24 15:10:09
 ///

#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"

#include <cstddef>
#include <queue>
#include <functional>

namespace my
{

class TaskQueue
{
    typedef std::function<void()> TaskCallback;
public:
    TaskQueue(const std::size_t & size = 10);

    bool full() const;
    bool empty() const;

    void push(const TaskCallback && elem);
    TaskCallback pop();

    void wakeUp();
private:
    std::size_t _size;
    std::queue<TaskCallback> _que;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    bool _flag;
};

} // end of namespace my

#endif
