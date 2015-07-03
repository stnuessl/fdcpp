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

#ifndef _FDCPP_FILE_HPP_
#define _FDCPP_FILE_HPP_

#include <string>

#include <fds/base/iofile_descriptor.hpp>

namespace fd {

class file : public iofile_descriptor {
public:
    explicit file(const char *path, int flags = 0);
    explicit file(const char *path, int flags, mode_t mode);
    explicit file(const std::string &path, int flags = 0);
    explicit file(const std::string &path, int flags, mode_t mode);
    file(const file &other) = delete;
    file(file &&other);
    
    virtual ~file();
    
    file &operator=(const file &other) = delete;
    file &operator=(file &&other);
    
    size_t lseek(off_t offset, int whence) const;
    
    void fchmod(mode_t mode) const;
    void fchown(uid_t uid, gid_t gid) const;
    void fstat(struct stat *st) const;
    void fsync() const;
    void ftruncate(size_t size = 0) const;
    long fpathconf(int name) const;
    
    // bool isatty();
};

}

#endif /* _FDCPP_FILE_HPP_ */