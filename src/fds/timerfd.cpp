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

#include <unistd.h>

#include <fds/timerfd.hpp>
#include <util/throw.hpp>

static const char *tag = "timerfd";

static fd::timerfd::timerspec to_timerspec(const struct itimerspec *its)
{
    int ns_per_sec = 1e9;
    
    auto a = its->it_value.tv_sec * ns_per_sec + its->it_value.tv_nsec;
    auto b = its->it_interval.tv_sec * ns_per_sec + its->it_interval.tv_nsec;
    
    return fd::timerfd::timerspec(a, b);
}

namespace fd {

timerfd::timerfd(clockid_t clockid, int flags)
    : file_descriptor(timerfd_create(clockid, flags))
{
}

timerfd::timerfd(timerfd &&other)
    : file_descriptor(std::move(other))
{
}

timerfd &timerfd::operator=(timerfd &&other)
{
    file_descriptor::operator=(std::move(other));
    
    return *this;
}

void timerfd::gettime(struct itimerspec *val) const
{
    auto err = timerfd_gettime(_fd, val);
    if (err < 0)
        throw_system_error(tag, "gettime()");
}

timerfd::timerspec timerfd::gettime() const
{
    struct itimerspec its;
    
    gettime(&its);
    
    return to_timerspec(&its);
}

void timerfd::settime(const struct itimerspec *spec, 
                      struct itimerspec *old, 
                      int flags) const
{
    auto err = timerfd_settime(_fd, flags, spec, old);
    if (err < 0)
        throw_system_error(tag, "settime()");
}

timerfd::timerspec timerfd::settime(const timerfd::timerspec &spec, 
                                    int flags) const
{
    int ns_per_sec = 1e9;
    struct itimerspec its, old;
    
    its.it_value.tv_sec     = spec.first / ns_per_sec;
    its.it_value.tv_nsec    = spec.first % ns_per_sec;
    its.it_interval.tv_sec  = spec.second / ns_per_sec;
    its.it_interval.tv_nsec = spec.second % ns_per_sec;
    
    settime(&its, &old, flags);
    
    return to_timerspec(&old);
}

uint64_t timerfd::read() const
{
    uint64_t val;
    size_t sum;
    ssize_t n;
    
    sum = 0;
    
    do {
        n = ::read(_fd, ((char *) &val) + sum, sizeof(val) - sum);
        if (n < 0)
            throw_system_error(tag, "read()");
        
        sum += n;
    } while (sum != sizeof(val));
    
    return val;
}

}