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

#ifndef _FDCPP_SIGNALFD_HPP_
#define _FDCPP_SIGNALFD_HPP_

#include <signal.h>
#include <sys/signalfd.h>

#include <fds/base/ifile_descriptor.hpp>

namespace fd {

class signalfd : public ifile_descriptor {
public:
    explicit signalfd(const sigset_t *mask, int fd = -1, int flags = 0);
    signalfd(const signalfd &other) = delete;
    signalfd(signalfd &&other);
    
    virtual ~signalfd() = default;
    
    signalfd &operator=(const signalfd &other) = delete;
    signalfd &operator=(signalfd &&other);
    
    void read(struct signalfd_siginfo *info);
};

}

#endif /* _FDCPP_SIGNALFD_HPP_ */