 ///
 /// @file    MutexLock.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-02 15:28:22
 ///

#include "./bo_threadpool/MutexLock.h"

namespace my
{

MutexLock::MutexLock()
: _isLocking(false)
{
	::pthread_mutex_init(&_mutex, NULL);
}

MutexLock::~MutexLock()
{
	pthread_mutex_destroy(&_mutex);
}

void MutexLock::lock()
{
	pthread_mutex_lock(&_mutex); // 可能阻塞。
	_isLocking = true;
}

void MutexLock::unlock()
{
	pthread_mutex_unlock(&_mutex);
	_isLocking = false;
}

pthread_mutex_t * MutexLock::getMutexPtr()
{ return &_mutex; }

bool MutexLock::isLocking() const
{ return _isLocking; }

} // end of namespace my
