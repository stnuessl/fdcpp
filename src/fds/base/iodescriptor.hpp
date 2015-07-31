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

#ifndef _FDCPP_IODESCRIPTOR_HPP_
#define _FDCPP_IODESCRIPTOR_HPP_

#include <fds/base/descriptor.hpp>

namespace fd {

class iodescriptor : public descriptor {
public:
    explicit iodescriptor(int fd);
    explicit iodescriptor(descriptor &&other);
    iodescriptor(const iodescriptor &other) = delete;
    iodescriptor(iodescriptor &&other) = default;
    
    virtual ~iodescriptor() = default;
    
    iodescriptor &operator=(const iodescriptor &other) = delete;
    iodescriptor &operator=(iodescriptor &&other) = default;
    
    size_t read(char *buffer, size_t size) const;
    size_t write(const char *buffer, size_t size) const;
    
    size_t pread(char *buffer, size_t size, size_t off) const;
    size_t pwrite(const char *buffer, size_t size, size_t off) const;
};

}

#endif /* _FDCPP_IODESCRIPTOR_HPP_ */