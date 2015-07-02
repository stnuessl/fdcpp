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

signalfd::signalfd(const sigset_t *mask, int fd, int flags)
    : file_descriptor(::signalfd(fd, mask, flags))
{
}

signalfd::signalfd(signalfd &&other)
    : file_descriptor(std::move(other))
{
}

signalfd &signalfd::operator=(signalfd &&other)
{
    file_descriptor::operator=(std::move(other));
    
    return *this;
}

void signalfd::read(struct signalfd_siginfo *info)
{
    size_t sum;
    ssize_t n;
    
    sum = 0;
    
    do {
        n = ::read(_fd, (char *) info + sum, sizeof(*info) - sum);
        if (n < 0)
            throw_system_error(tag, "read()");
        
        sum += n;
    } while (sum != sizeof(*info));
}


}