 ///
 /// @file    Threadpool.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-24 19:12:50
 ///

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Thread.h"
#include "TaskQueue.h"

#include <cstddef>

#include <vector>
#include <memory>
#include <functional>

namespace my
{

class Threadpool
{
    typedef std::function<void()> TaskCallback;
public:
    Threadpool(const std::size_t & threadNums = 4, const std::size_t & queSize = 10);
    ~Threadpool();

    void start();
    void stop();

    void addTask(TaskCallback && cb);
private:
    TaskCallback getTask();
    void threadFunc();
private:
    std::size_t _threadNums;
    std::size_t _queSize;
    TaskQueue _taskQue;
    std::vector<std::shared_ptr<Thread> > _threads;
    bool _isExit;
};

} // end of namespace my

#endif
