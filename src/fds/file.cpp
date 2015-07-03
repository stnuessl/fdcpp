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

#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

#include <utility>
#include <cerrno>

#include <fds/file.hpp>
#include <util/throw.hpp>

static const char *tag = "file";

namespace fd {

file::file(const char *path, int flags)
    : iofile_descriptor(open(path, flags))
{

}


file::file(const char *path, int flags, mode_t mode)
    : iofile_descriptor(open(path, flags, mode))
{

}


file::file(const std::string &path, int flags)
    : file(path.c_str(), flags)
{

}


file::file(const std::string &path, int flags, mode_t mode)
    : file(path.c_str(), flags, mode)
{

}

file::file(file &&other)
    : iofile_descriptor(std::move(other))
{
}

file::~file()
{
}

file &file::operator=(file &&other)
{
    iofile_descriptor::operator=(std::move(other));

    return *this;
}

void file::fchmod(mode_t mode) const
{
    auto err = ::fchmod(_fd, mode);
    if (err < 0)
        throw_system_error(tag, "fchmod()");
}

void file::fchown(uid_t uid, gid_t gid) const
{
    auto err = ::fchown(_fd, uid, gid);
    if (err < 0)
        throw_system_error(tag, "fchown()");
}


size_t file::lseek(off_t offset, int whence) const
{
    auto n = ::lseek(_fd, offset, whence);
    if (n < 0)
        throw_system_error(tag, "lseek()");
    
    return static_cast<size_t>(n);
}

void file::fstat(struct stat *st) const
{
    auto err = fstat(_fd, st);
    if (err < 0)
        throw_system_error(tag, "fstat()");
}

void file::fsync() const
{
    auto err = ::fsync(_fd);
    if (err < 0)
        throw_system_error(tag, "fsync()");
}

void file::ftruncate(size_t size) const
{
    auto err = ::ftruncate(_fd, static_cast<off_t>(size));
    if (err < 0)
        throw_system_error(tag, "truncate()");
}


}