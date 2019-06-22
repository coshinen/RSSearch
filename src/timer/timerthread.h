 ///
 /// @file    timerthread.h
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-05 17:57:35
 ///

#ifndef __TIMERTHREAD_H__
#define __TIMERTHREAD_H__

#include "timer.h"
#include "thread.h"

namespace md
{

class TimerThread
{
    typedef std::function<void()> TimerCallback;
public:
    TimerThread(TimerCallback && cb, time_t initialTime, time_t intervalTime);

    void start();
    void stop();
private:
    Timer _timer;
    Thread _thread;
};

} // end of namespace md

#endif
