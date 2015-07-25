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

namespace fd {

timerfd::timerfd(clockid_t clockid, int flags)
    : ifile_descriptor(timerfd_create(clockid, flags))
{
}

timerfd::timerfd(const timerfd &other)
    : ifile_descriptor(::dup(other._fd))
{
    if (_fd < 0)
        throw_system_error(tag, "dup()");
}

timerfd::timerfd(timerfd &&other)
    : ifile_descriptor(std::move(other))
{
}

timerfd &timerfd::operator=(timerfd &&other)
{
    ifile_descriptor::operator=(std::move(other));
    
    return *this;
}

timerfd timerfd::dup() const
{
    return timerfd(*this);
}


void timerfd::gettime(struct itimerspec &val) const
{
    auto err = timerfd_gettime(_fd, &val);
    if (err < 0)
        throw_system_error(tag, "gettime()");
}

void timerfd::settime(const struct itimerspec *spec, 
                      struct itimerspec *old, 
                      int flags) const
{
    auto err = timerfd_settime(_fd, flags, spec, old);
    if (err < 0)
        throw_system_error(tag, "settime()");
}

void timerfd::settime(const struct itimerspec &spec, 
                      struct itimerspec &old, 
                      int flags) const
{
    settime(&spec, &old, flags);
}

void timerfd::settime(const struct itimerspec &spec, int flags) const
{
    settime(&spec, nullptr, flags);
}

uint64_t timerfd::read() const
{
    uint64_t val;
    ssize_t n = 0;

    do {
        n += ifile_descriptor::read(((char *) &val) + n, sizeof(val) - n);
    } while (n != sizeof(val));
    
    return val;
}

}