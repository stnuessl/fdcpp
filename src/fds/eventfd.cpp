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

#include <utility>

#include <fds/eventfd.hpp>
#include <util/throw.hpp>

static const char *tag = "eventfd";

namespace fd {

eventfd::eventfd(unsigned int initval, int flags)
    : iodescriptor(::eventfd(initval, flags))
{
    if (_fd < 0)
        throw_system_error(tag, "eventfd()");
}

eventfd::eventfd(descriptor &&other)
    : iodescriptor(std::move(other))
{
    if (_fd < 0)
        throw_system_error(tag, "eventfd()", EBADF);
}

eventfd::eventfd(const eventfd &other)
    : iodescriptor(::dup(other._fd))
{
    if (_fd < 0)
        throw_system_error(tag, "dup()");
}

const eventfd &eventfd::operator=(const eventfd &other) const
{
    int err = ::dup2(other._fd, _fd);
    if (err < 0)
        throw_system_error(tag, "dup2()");
    
    return *this;
}

eventfd eventfd::dup() const
{
    return eventfd(*this);
}

void eventfd::dup2(const eventfd &other) const
{
    *this = other;
}

uint64_t eventfd::read() const
{
    uint64_t val;
    size_t n;
    
    n = 0;
    
    do {
        n += iodescriptor::read(((char *) &val) + n, sizeof(val) - n);
    } while (n != sizeof (val));
    
    return val;
}

void eventfd::write(uint64_t val) const
{
    size_t n = 0;
    
    do {
        n += iodescriptor::write(((char *) &val) + n, sizeof(val) - n);
    } while (n != sizeof(val));
}

}

