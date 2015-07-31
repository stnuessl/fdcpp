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
    : descriptor(epoll_create1(flags))
{
    if (_fd < 0)
        throw_system_error(tag, "epoll()");
}

epoll::epoll(const epoll &other)
    : descriptor(::dup(other._fd))
{
    if (_fd < 0)
        throw_system_error(tag, "dup()");
}

epoll::epoll(descriptor &&other)
    : descriptor(std::move(other))
{
    if (_fd < 0)
        throw_system_error(tag, "epoll()", EBADF);
}

const epoll &epoll::operator=(const epoll &other) const
{
    int err = ::dup2(other._fd, _fd);
    if (err < 0)
        throw_system_error(tag, "dup2()");
    
    return *this;
}

epoll epoll::dup() const
{
    return epoll(*this);
}

void epoll::dup2(const epoll &other) const
{
    *this = other;
}

void epoll::ctl(int op, int fd, struct epoll_event *ev) const
{
    auto err = epoll_ctl(_fd, op, fd, ev);
    if (err < 0)
        throw_system_error(tag, "ctl()");
}

void epoll::ctl(int op, int fd, struct epoll_event &ev) const
{
    ctl(op, fd, &ev);
}

void epoll::ctl(int op, int fd, uint32_t event_mask, int data) const
{
    struct epoll_event ev;
    
    /* initialize memory of data field */
    ev.data.u64 = 0;
    
    ev.data.fd = data;
    ev.events = event_mask;
    
    ctl(op, fd, &ev);
}

void epoll::ctl(int op, int fd, uint32_t event_mask, uint32_t data) const
{
    struct epoll_event ev;
    
    /* initialize memory of data field */
    ev.data.u64 = 0;
    
    ev.data.u32 = data;
    ev.events = event_mask;
    
    ctl(op, fd, &ev);
}

void epoll::ctl(int op, int fd, uint32_t event_mask, uint64_t data) const
{
    struct epoll_event ev;
    
    ev.data.u64 = data;
    ev.events = event_mask;
    
    ctl(op, fd, &ev);
}

void epoll::ctl(int op, int fd, uint32_t event_mask, void *data) const
{
    struct epoll_event ev;
    
    ev.data.ptr = data;
    ev.events = event_mask;
    
    ctl(op, fd, &ev);
}

void epoll::add(int fd, uint32_t event_mask, int data) const
{
    ctl(EPOLL_CTL_ADD, fd, event_mask, data);
}

void epoll::add(int fd, uint32_t event_mask, uint32_t data) const
{
    ctl(EPOLL_CTL_ADD, fd, event_mask, data);
}

void epoll::add(int fd, uint32_t event_mask, uint64_t data) const
{
    ctl(EPOLL_CTL_ADD, fd, event_mask, data);
}

void epoll::add(int fd, uint32_t event_mask, void *data) const
{
    ctl(EPOLL_CTL_ADD, fd, event_mask, data);
}

void epoll::mod(int fd, uint32_t event_mask, int data) const
{
    ctl(EPOLL_CTL_MOD, fd, event_mask, data);
}

void epoll::mod(int fd, uint32_t event_mask, uint32_t data) const
{
    ctl(EPOLL_CTL_MOD, fd, event_mask, data);
}

void epoll::mod(int fd, uint32_t event_mask, uint64_t data) const
{
    ctl(EPOLL_CTL_MOD, fd, event_mask, data);
}

void epoll::mod(int fd, uint32_t event_mask, void *data) const
{
    ctl(EPOLL_CTL_MOD, fd, event_mask, data);
}

void epoll::del(int fd) const
{
    ctl(EPOLL_CTL_DEL, fd);
}

int epoll::wait(epoll_event *events, int size, int timeout) const
{
    auto n = epoll_wait(_fd, events, size, timeout);
    if (n < 0)
        throw_system_error(tag, "wait()");
    
    return n;
}

int epoll::pwait(struct epoll_event *events, 
                 int size, 
                 const sigset_t &sigmask, 
                 int timeout_ms) const
{
    auto n = epoll_pwait(_fd, events, size, timeout_ms, &sigmask);
    if (n < 0)
        throw_system_error(tag, "pwait()");
    
    return n;
}


}