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

#include <sys/statvfs.h>

#include <string>

#include <fds/base/iodescriptor.hpp>

namespace fd {

class file : public iodescriptor {
public:
    file(const char *path, int flags = 0);
    file(const char *path, int flags, mode_t mode);
    file(const std::string &path, int flags = 0);
    file(const std::string &path, int flags, mode_t mode);
    explicit file(descriptor &&other);
    file(file &&other) = default;
    
    virtual ~file() = default;
    
    file dup() const;
    void dup2(const file &other) const;
    
    file &operator=(file &&other) = default;
    
    size_t lseek(off_t offset, int whence) const;
    
    void fchmod(mode_t mode) const;
    void fchown(uid_t uid, gid_t gid) const;
    void fstat(struct stat &st) const;
    void ftruncate(size_t size = 0) const;
    void fsync() const;
    long fpathconf(int name) const;
    void fdatasync() const;
    void fstatvfs(struct statvfs &svfs) const;
    
private:
    file(const file &other);
    const file &operator=(const file &other) const;
};

}

#endif /* _FDCPP_FILE_HPP_ */