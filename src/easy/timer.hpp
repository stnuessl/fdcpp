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

#ifndef _FD_TIMER_HPP_
#define _FD_TIMER_HPP_

#include <fds/timerfd.hpp>

namespace fd {
namespace easy {

class timer : public timerfd {
public:
    enum state {
        STARTED,
        PAUSED,
        STOPPED
    };
    
    explicit timer();
    explicit timer(const timerspec &ts);
    timer(const timer &other) = delete;
    timer(timer &&other);
    
    virtual ~timer() = default;
    
    timer &operator=(const timer &other) = delete;
    timer &operator=(timer &&other);
    
    void settime(const timerspec &ts);
    timerspec gettime() const;
    
    void start();
    void pause();
    void resume();
    void stop();
    
    uint64_t timeouts() const;
    
    timer::state state() const;
private:
    timerspec _spec;
    uint64_t _remaining;
    
    enum state _state;
};

}
}

#endif /* _FD_TIMER_HPP_ */