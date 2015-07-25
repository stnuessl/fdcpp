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

#include <fds/base/ifile_descriptor.hpp>

namespace fd {

class timerfd : public ifile_descriptor {
public:
    explicit timerfd(clockid_t clockid = CLOCK_MONOTONIC, int flags = 0);
    timerfd(timerfd &&other);
    
    virtual ~timerfd() = default;
    
    timerfd dup() const;
    
    timerfd &operator=(const timerfd &other) = delete;
    timerfd &operator=(timerfd &&other);
    
    void gettime(struct itimerspec &val) const;
    
    void settime(const struct itimerspec &spec, 
                 struct itimerspec &old, 
                 int flags = 0) const;
    void settime(const struct itimerspec &spec, int flags = 0) const;

    uint64_t read() const;
private:
    timerfd(const timerfd &other);
    
    void settime(const struct itimerspec *spec, 
                 struct itimerspec *old, 
                 int flags) const;
};

}

#endif /* _FDCPP_TIMERFD_HPP_ */