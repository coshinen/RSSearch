 ///
 /// @file    TaskQueue.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-24 15:22:37
 ///

#include "./bo_threadpool/TaskQueue.h"

namespace my
{

TaskQueue::TaskQueue(const std::size_t & size)
: _size(size)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _flag(true)
{}

bool TaskQueue::full() const
{ return _size == _que.size(); }

bool TaskQueue::empty() const
{ return 0 == _que.size(); }

void TaskQueue::push(const TaskCallback && elem)
{
	MutexLockGuard mlg(_mutex);

	while (full())
		_notFull.wait();

	_que.push(std::move(elem));
	_notEmpty.notify();
}

TaskQueue::TaskCallback TaskQueue::pop()
{
	MutexLockGuard mlg(_mutex);
	
	while (_flag && empty())
		_notEmpty.wait();

	if (_flag) {
		TaskCallback elem = _que.front();
		_que.pop();
		_notFull.notify();
	
		return elem;
	} else
		return NULL;
}

void TaskQueue::wakeUp()
{
	_flag = false;
	_notEmpty.notifyAll();
}

} // end of namespace my
