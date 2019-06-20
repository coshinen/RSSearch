 ///
 /// @file    Timer.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-05 17:03:19
 ///

#ifndef __TIMER_H__
#define __TIMER_H__

#include <functional>

namespace my
{

class Timer
{
	typedef std::function<void()> TimerCallback;
public:
	Timer(TimerCallback && cb, time_t initialTime, time_t intervalTime);
	~Timer();

	void start();
	void stop();
private:
	int createTimerfd();
	void setTimerfd(time_t initialTime, time_t intervalTime);
	void handleRead();
private:
	int _timefd;
	TimerCallback _cb;
	time_t _initialTime;
	time_t _intervalTime;
	bool _isStarted;
};

} // end of namespace my

#endif
