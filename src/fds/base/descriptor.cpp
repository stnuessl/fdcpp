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
#include <fcntl.h>

#include <utility>

#include <fds/base/descriptor.hpp>

#include <util/throw.hpp>

static const char *tag = "descriptor";

namespace fd {

descriptor::descriptor(int fd)
    : _fd(fd)
{
}


descriptor::descriptor(descriptor &&other)
    : _fd(other._fd)
{
    other._fd = -1;
}

descriptor::~descriptor()
{
    if (_fd >= 0)
        ::close(_fd);
}

descriptor &descriptor::operator=(descriptor &&other)
{
    std::swap(_fd, other._fd);
    
    return *this;
}

descriptor::operator int() const
{
    return _fd;
}


int descriptor::fcntl(int cmd)
{
    int ret = ::fcntl(_fd, cmd);
    if (ret < 0)
        throw_system_error(tag, "fcntl()");
    
    return ret;
}

int descriptor::fcntl(int cmd, int arg)
{
    int ret = ::fcntl(_fd, cmd, arg);
    if (ret < 0)
        throw_system_error(tag, "fcntl()");
    
    return ret;
}

int descriptor::fcntl(int cmd, struct flock *arg)
{
    int ret = ::fcntl(_fd, cmd, arg);
    if (ret < 0)
        throw_system_error(tag, "fcntl()");
    
    return ret;
}

int descriptor::fd() const
{
    return _fd;
}


}