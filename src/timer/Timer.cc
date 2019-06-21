 ///
 /// @file    Timer.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-11-05 17:10:48
 ///
 
#include "./timer/Timer.h"

#include <stdio.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>

#include <iostream>
#include <utility>

namespace md
{

Timer::Timer(TimerCallback && cb, time_t initialTime, time_t intervalTime)
: _timefd(createTimerfd())
, _cb(std::move(cb))
, _initialTime(initialTime)
, _intervalTime(intervalTime)
, _isStarted(false)
{}

Timer::~Timer()
{
    if (_isStarted)
        stop();
}

void Timer::start()
{
    struct pollfd pfd;
    pfd.fd = _timefd;
    pfd.events = POLLIN;

    _isStarted = true;
    setTimerfd(_initialTime, _intervalTime);
    while (_isStarted)
    {
        std::cout << "timer run" << std::endl;
        int nready = ::poll(&pfd, 1, 5000);
        if (-1 == nready && EINTR == errno)
            continue;
        else if (-1 == nready) {
            std::cout << "poll error!" << std::endl;
            return;
        } else if (0 == nready) {
            std::cout << "poll timeout!" << std::endl;
        } else {
            if (_timefd == pfd.fd && (pfd.revents & POLLIN)) {
                handleRead();
                if (_cb)
                    _cb();
            }
        }
    }
}

void Timer::stop()
{
    if (_isStarted) {
        _isStarted = false;
        setTimerfd(0, 0);
        ::close(_timefd);
    }
}
    
int Timer::createTimerfd()
{
    int fd = ::timerfd_create(CLOCK_REALTIME, 0);
    if (-1 == fd) {
        ::perror("timerfd_create error");
    }
    return fd;
}

void Timer::setTimerfd(time_t initialTime, time_t intervalTime)
{
    struct itimerspec value;
    value.it_value.tv_sec = initialTime;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = intervalTime;
    value.it_interval.tv_nsec = 0;

    int ret = timerfd_settime(_timefd, 0, &value, NULL);
    if (-1 == ret) {
        ::perror("timerfd_settime error");
    }
}

void Timer::handleRead()
{
    uint64_t howmany;
    int ret = ::read(_timefd, &howmany, sizeof(howmany));
    if (ret != sizeof(howmany)) {
        ::perror("read error");
    }
}

} // end of namespace md
