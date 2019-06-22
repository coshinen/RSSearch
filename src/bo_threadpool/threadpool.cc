 ///
 /// @file    threadpool.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-24 19:23:13
 ///

#include "threadpool.h"
#include "configuration.h"

#include <unistd.h>

namespace md
{

Threadpool::Threadpool(const std::size_t & threadNums, const std::size_t & queSize)
: _threadNums(threadNums)
, _queSize(queSize)
, _taskQue(_queSize)
, _isExit(false)
{
    _threads.reserve(_threadNums);
}

Threadpool::~Threadpool()
{
    if (!_isExit)
        stop();
}

void Threadpool::start()
{
    for (std::size_t idx = 0; idx != _threadNums; ++idx)
    {
        _threads.push_back(std::shared_ptr<Thread>(new Thread(std::bind(&Threadpool::threadFunc, this), uint2str(idx))));
    }

    for (auto & thread : _threads)
    {
        thread->start();
    }
}

void Threadpool::stop()
{
    if (!_isExit) {
        while (!_taskQue.empty())
        {
            ::sleep(2);
        }

        _isExit = true;

        _taskQue.wakeUp();

        for (auto & thread : _threads)
        {
            thread->join();
        }
    }
}

void Threadpool::addTask(TaskCallback && cb)
{ _taskQue.push(std::move(cb)); }

Threadpool::TaskCallback Threadpool::getTask()
{ return _taskQue.pop(); }

void Threadpool::threadFunc()
{
    while (!_isExit)
    {
        TaskCallback task = getTask();
        if (task)
            task();
    }
}

} // end of namespace md
