 ///
 /// @file    timerthread.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-05 18:01:15
 ///
 
#include "timerthread.h"

#include <iostream>
#include <utility>

namespace md
{

TimerThread::TimerThread(TimerCallback && cb, time_t initialTime, time_t intervalTime)
: _timer(std::move(cb), initialTime, intervalTime)
, _thread(std::bind(&Timer::start, &_timer))
{}

void TimerThread::start()
{
    _thread.start();
    std::cout << "---- timer start ----" << std::endl;
}

void TimerThread::stop()
{
    _timer.stop();
    std::cout << "---- timer stop ----" << std::endl;
    _thread.join();
}

} // end of namespace md
