/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Steffen Nuessle
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <utility>

#include <easy/timer.hpp>

namespace fd {

namespace easy {

timer::timer()
    : timerfd(),
      _state(STOPPED)
{
    _spec.it_value.tv_sec = 0;
    _spec.it_value.tv_nsec = 0;
    _spec.it_interval.tv_sec = 0;
    _spec.it_interval.tv_nsec = 0;
    
    _remaining.it_value.tv_sec = 0;
    _remaining.it_value.tv_nsec = 0;
    _remaining.it_interval.tv_sec = 0;
    _remaining.it_interval.tv_nsec = 0;
}

timer::timer(timer &&other)
    : timerfd(std::move(other)),
      _state(other._state)
{
    _spec.it_value.tv_sec = other._spec.it_value.tv_sec;
    _spec.it_value.tv_nsec = other._spec.it_value.tv_nsec;
    _spec.it_interval.tv_sec = other._spec.it_interval.tv_sec;
    _spec.it_interval.tv_nsec = other._spec.it_interval.tv_nsec;
    
    _remaining.it_value.tv_sec = other._remaining.it_value.tv_sec;
    _remaining.it_value.tv_nsec = other._remaining.it_value.tv_nsec;
    _remaining.it_interval.tv_sec = other._remaining.it_interval.tv_sec;
    _remaining.it_interval.tv_nsec = other._remaining.it_interval.tv_nsec;
}

timer::timer(const struct itimerspec &ts)
    : timerfd(),
      _state(STOPPED)
{
    settime(ts);
}

timer &timer::operator=(timer &&other)
{
    timerfd::operator=(std::move(other));
    
    _spec.it_value.tv_sec = other._spec.it_value.tv_sec;
    _spec.it_value.tv_nsec = other._spec.it_value.tv_nsec;
    _spec.it_interval.tv_sec = other._spec.it_interval.tv_sec;
    _spec.it_interval.tv_nsec = other._spec.it_interval.tv_nsec;
    
    _remaining.it_value.tv_sec = other._remaining.it_value.tv_sec;
    _remaining.it_value.tv_nsec = other._remaining.it_value.tv_nsec;
    _remaining.it_interval.tv_sec = other._remaining.it_interval.tv_sec;
    _remaining.it_interval.tv_nsec = other._remaining.it_interval.tv_nsec;
    
    _state = other._state;
    
    return *this;
}

void timer::settime(const struct itimerspec &ts)
{
    if (ts.it_value.tv_sec || ts.it_value.tv_nsec) {
       _spec.it_value.tv_sec = ts.it_value.tv_sec;
       _spec.it_value.tv_nsec = ts.it_value.tv_nsec;
    } else {
        _spec.it_value.tv_sec = ts.it_interval.tv_sec;
        _spec.it_value.tv_nsec = ts.it_interval.tv_nsec;
    }
    
    _spec.it_interval.tv_sec = ts.it_interval.tv_sec;
    _spec.it_interval.tv_nsec = ts.it_interval.tv_nsec;
}

void timer::start()
{
    if (_state != STARTED) {
        (void) timerfd::settime(_spec);
        
        _state = STARTED;
    }
}

void timer::pause()
{
    struct itimerspec its;
    
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    
    if (_state == STARTED) {
        timerfd::settime(its, _remaining);
        _state = PAUSED;
    }
}

void timer::resume()
{
    if (_state == PAUSED) {
        if (!_remaining.it_value.tv_sec && !_remaining.it_value.tv_nsec) {
            _remaining.it_value.tv_sec = _remaining.it_interval.tv_sec;
            _remaining.it_value.tv_nsec = _remaining.it_interval.tv_nsec;
        }

        timerfd::settime(_remaining);
        _state = STARTED;
    }
}

void timer::stop()
{
    struct itimerspec its;
    
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    
    if (_state != STOPPED) {
        timerfd::settime(its);
        _state = STOPPED;
    }
}

uint64_t timer::timeouts() const
{
    return read();
}

enum timer::state timer::state() const
{
    return _state;
}

}

}
