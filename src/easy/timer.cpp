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
      _spec(0, 0),
      _remaining(0),
      _state(STOPPED)
{
}

timer::timer(timer &&other)
    : timerfd(std::move(other)),
      _spec(std::move(other._spec)),
      _remaining(other._remaining),
      _state(other._state)
{
}

timer::timer(const timerfd::timerspec &ts)
    : timerfd(),
      _spec(ts),
      _remaining(0),
      _state(STOPPED)
{

}


timer &timer::operator=(timer &&other)
{
    timerfd::operator=(std::move(other));
    _spec = std::move(other._spec);
    _remaining = other._remaining;
    _state = other._state;
    
    return *this;
}

void timer::settime(const timerspec &ts)
{
    _spec.first = (ts.first) ? ts.first : ts.second;
    _spec.second = ts.second;
}

timerfd::timerspec timer::gettime() const
{
    return timerfd::gettime();
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
    if (_state == STARTED) {
        _remaining = timerfd::settime({0, 0}).first;
        _state = PAUSED;
    }
}

void timer::resume()
{
    if (_state == PAUSED) {
        (void) timerfd::settime({_remaining, _spec.second});
        _state = STARTED;
    }
}

void timer::stop()
{
    if (_state != STOPPED) {
        (void) timerfd::settime({0, 0});
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
