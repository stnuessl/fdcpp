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

#include <fds/signalfd.hpp>
#include <util/throw.hpp>

static const char *tag = "signalfd";

namespace fd {

signalfd::signalfd(const sigset_t &mask, int flags)
    : idescriptor(::signalfd(-1, &mask, flags))
{
    if (_fd < 0)
        throw_system_error(tag, "signalfd()");
}

signalfd::signalfd(descriptor &&other)
    : idescriptor(std::move(other))
{
    if (_fd < 0)
        throw_system_error(tag, "signalfd()", EBADF);
}

signalfd::signalfd(const signalfd &other)
    : idescriptor(::dup(other._fd))
{
    if (_fd < 0)
        throw_system_error(tag, "dup()");
}

const signalfd &signalfd::operator=(const signalfd &other) const
{
    int err = ::dup2(other._fd, _fd);
    if (err < 0)
        throw_system_error(tag, "dup2()");
    
    return *this;
}

void signalfd::setmask(const sigset_t &mask) const
{
    int err = ::signalfd(_fd, &mask, 0);
    if (err < 0)
        throw_system_error(tag, "set()");
}


signalfd signalfd::dup() const
{
    return signalfd(*this);
}

void signalfd::dup2(const signalfd &other) const
{
    *this = other;
}

void signalfd::read(struct signalfd_siginfo &info) const
{
    size_t n = 0;
    
    do {
        n += idescriptor::read((char *) &info + n, sizeof(info) - n);
    } while (n != sizeof(info));

}


}