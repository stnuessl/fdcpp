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

#include <fds/epoll.hpp>
#include <util/throw.hpp>

static const char *tag = "epoll";
    
namespace fd {

epoll::epoll(int flags)
    : file_descriptor(epoll_create1(flags))
{
}


epoll::epoll(epoll &&other)
    : file_descriptor(std::move(other._fd))
{
    other._fd = -1;
}

epoll &epoll::operator=(epoll &&other)
{
    file_descriptor::operator=(std::move(other));
    
    return *this;
}

void epoll::ctl(int op, int fd, struct epoll_event *ev) const
{
    auto err = epoll_ctl(_fd, op, fd, ev);
    if (err < 0)
        throw_system_error(tag, "ctl()");
}



void epoll::ctl(int op, int fd, void *ptr, uint32_t events) const
{
    struct epoll_event ev;
    
    ev.data.ptr = ptr;
    ev.events   = events;
    
    ctl(op, fd, &ev);
}

void epoll::ctl(int op, int fd, uint32_t u32, uint32_t events) const
{
    struct epoll_event ev;
    
    ev.data.u64 = 0;
    ev.data.u32 = u32;
    ev.events   = events;
    
    ctl(op, fd, &ev);
}

void epoll::ctl(int op, int fd, uint64_t u64, uint32_t events) const
{
    struct epoll_event ev;
    
    ev.data.u64 = u64;
    ev.events   = events;
    
    ctl(op, fd, &ev);
}

void epoll::ctl(int op, const file_descriptor &desc, epoll_event *ev) const
{
    ctl(op, desc.fd(), ev);
}


void epoll::ctl(int op, 
                const file_descriptor &desc, 
                void *ptr, 
                uint32_t events) const
{
    ctl(op, desc.fd(), ptr, events);
}

void epoll::ctl(int op, 
                const file_descriptor &desc, 
                uint32_t u32, 
                uint32_t events) const
{
    ctl(op, desc.fd(), u32, events);
}

void epoll::ctl(int op, 
                const file_descriptor &desc, 
                uint64_t u64, 
                uint32_t events) const
{
    ctl(op, desc.fd(), u64, events);
}

int epoll::wait(epoll_event *events, int size, int timeout) const
{
    auto n = epoll_wait(_fd, events, size, timeout);
    if (n < 0)
        throw_system_error(tag, "wait()");
    
    return n;
}

}