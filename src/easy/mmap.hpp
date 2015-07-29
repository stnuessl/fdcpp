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

#ifndef _FDCPP_MMAP_HPP_
#define _FDCPP_MMAP_HPP_

#include <sys/mman.h>

#include <fds/file.hpp>

namespace fd {

namespace easy {

class mmap {
public:
    explicit mmap(size_t len, int prot, int flags, int fd, off_t off = 0);
    explicit mmap(void *addr, 
                  size_t len, 
                  int prot, 
                  int flags, 
                  int fd, 
                  off_t off = 0);
    explicit mmap(const file &file, int prot, int flags, off_t off = 0);
    mmap(const mmap &other) = delete;
    mmap(mmap &&other);
    
    ~mmap();
    
    mmap &operator=(const mmap &other) = delete;
    mmap &operator=(mmap &&other);
    
    char &operator[](size_t i);
    const char &operator[](size_t i) const;
    
    operator char *();
    operator const char *() const;
    
    size_t size() const;
private:
    char *_addr;
    size_t _size;
};

}
}

#endif /* _FDCPP_MMAP_HPP_ */