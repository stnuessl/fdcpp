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

#include <fds/base/iodescriptor.hpp>
#include <util/throw.hpp>

static const char *tag = "iodescriptor";

namespace fd {

iodescriptor::iodescriptor(int fd)
    : descriptor(fd)
{
}

iodescriptor::iodescriptor(descriptor &&other)
    : descriptor(std::move(other))
{
}

size_t iodescriptor::read(char *buffer, size_t size) const
{
    auto n = ::read(_fd, buffer, size);
    if (n < 0)
        throw_system_error(tag, "read()");
    
    return static_cast<size_t>(n);
}

size_t iodescriptor::write(const char *buffer, size_t size) const
{
    auto n = ::write(_fd, buffer, size);
    if (n < 0)
        throw_system_error(tag, "write()");
    
    return static_cast<size_t>(n);
}

size_t iodescriptor::pread(char *buffer, size_t size, size_t off) const
{
    auto n = ::pread(_fd, buffer, size, static_cast<off_t>(off));
    if (n < 0)
        throw_system_error(tag, "pread()");
    
    return static_cast<size_t>(n);
}

size_t iodescriptor::pwrite(const char *buffer, 
                                 size_t size, 
                                 size_t off) const
{
    auto n = ::pwrite(_fd, buffer, size, static_cast<off_t>(off));
    if (n < 0)
        throw_system_error(tag, "pwrite()");
    
    return static_cast<size_t>(n);
}


}