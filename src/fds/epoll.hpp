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

#ifndef _FDCPP_EPOLL_HPP_
#define _FDCPP_EPOLL_HPP_

#include <sys/epoll.h>

#include <fds/base/file_descriptor.hpp>

namespace fd {

class epoll : public file_descriptor {
public:
    explicit epoll(int flags = 0);
    epoll(const epoll &other) = delete;
    epoll(epoll &&other);
    
    virtual ~epoll() = default;
    
    epoll &operator=(const epoll &other) = delete;
    epoll &operator=(epoll &&other);
    
    void ctl(int op, int fd, struct epoll_event *ev) const;
    void ctl(int op, int fd, void *ptr, uint32_t events = EPOLLIN) const;
    void ctl(int op, int fd, uint32_t u32, uint32_t = EPOLLIN) const; 
    void ctl(int op, int fd, uint64_t u64, uint32_t = EPOLLIN) const;
    
    void ctl(int op, const file_descriptor &desc, struct epoll_event *ev) const;
    void ctl(int op, 
             const file_descriptor &desc, 
             void *ptr = nullptr, 
             uint32_t events = EPOLLIN) const;
    void ctl(int op, 
             const file_descriptor &desc,
             uint32_t u32, 
             uint32_t events = EPOLLIN) const; 
    void ctl(int op, 
             const file_descriptor &desc, 
             uint64_t u64, 
             uint32_t events = EPOLLIN) const;
    
    int wait(struct epoll_event *events, int size, int timeout_ms = -1) const;
};

}

#endif /* _FDCPP_EPOLL_HPP_ */