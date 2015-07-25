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
    : iofile_descriptor(::eventfd(initval, flags))
{
    if (_fd < 0)
        throw_system_error(tag, "eventfd()");
}

eventfd::eventfd(const eventfd &other)
    : iofile_descriptor(::dup(other._fd))
{
    if (_fd < 0)
        throw_system_error(tag, "dup()");
}


eventfd::eventfd(eventfd &&other)
    : iofile_descriptor(std::move(other))
{
}

eventfd &eventfd::operator=(eventfd &&other)
{
    iofile_descriptor::operator=(std::move(other));
    
    return *this;
}

eventfd eventfd::dup() const
{
    return eventfd(*this);
}

uint64_t eventfd::read() const
{
    uint64_t val;
    size_t n;
    
    n = 0;
    
    do {
        n += iofile_descriptor::read(((char *) &val) + n, sizeof(val) - n);
    } while (n != sizeof (val));
    
    return val;
}

void eventfd::write(uint64_t val) const
{
    size_t n = 0;
    
    do {
        n += iofile_descriptor::write(((char *) &val) + n, sizeof(val) - n);
    } while (n != sizeof(val));
}

}

