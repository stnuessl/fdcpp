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

#ifndef _FDCPP_TIMERFD_HPP_
#define _FDCPP_TIMERFD_HPP_

#include <sys/timerfd.h>
#include <time.h>

#include <utility>
#include <cstdint>

#include <fds/file_descriptor.hpp>

namespace fd {

class timerfd : public file_descriptor {
public:
    typedef std::pair<uint64_t, uint64_t> timerspec;
    
    explicit timerfd(clockid_t clockid = CLOCK_MONOTONIC, int flags = 0);
    timerfd(const timerfd &other) = delete;
    timerfd(timerfd &&other);
    
    virtual ~timerfd() = default;
    
    timerfd &operator=(const timerfd &other) = delete;
    timerfd &operator=(timerfd &&other);
    
    void gettime(struct itimerspec *val) const;
    timerspec gettime() const;
    
    void settime(const struct itimerspec *spec, 
                 struct itimerspec *old = nullptr, 
                 int flags = 0) const;
    
    timerspec settime(const timerspec &spec, int flags = 0) const;
  
    uint64_t read() const;
};

}

#endif /* _FDCPP_TIMERFD_HPP_ */