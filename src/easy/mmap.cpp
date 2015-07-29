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

#include <utility>

#include <easy/mmap.hpp>
#include <util/throw.hpp>

static const char *tag = "mmap";

namespace fd {

namespace easy {

mmap::mmap(size_t len, int prot, int flags, int fd, off_t off)
    : mmap(nullptr, len, prot, flags, fd, off)
{

}

mmap::mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)
    : _addr(static_cast<char *>(::mmap(addr, len, prot, flags, fd, off))),
      _size(len)
{
    if (_addr == MAP_FAILED)
        throw_system_error(tag, "mmap()");
}

mmap::mmap(const file &file, int prot, int flags, off_t off)
    : _addr(nullptr),
      _size(0)
{
    struct stat st;
    
    file.fstat(st);
    
    _size = st.st_size;
    _addr = static_cast<char *>(::mmap(nullptr, _size, prot, flags, file, off));
    if (_addr == MAP_FAILED)
        throw_system_error(tag, "mmap()");
}


// mmap::mmap(const mmap &other)
// {
// 
// }

mmap::mmap(mmap &&other)
    : _addr(other._addr),
      _size(other._size)
{
    other._addr = nullptr;
    other._size = 0;
}

mmap::~mmap()
{
    if (_addr)
        munmap(_addr, _size);
}

// mmap &mmap::operator=(const mmap &other)
// {
//     return *this;
// }

mmap &mmap::operator=(mmap &&other)
{
    std::swap(_addr, other._addr);
    std::swap(_size, other._size);
    
    return *this;
}

char &mmap::operator[](size_t i)
{
    return _addr[i];
}

const char &mmap::operator[](size_t i) const
{
    return _addr[i];
}

mmap::operator char *()
{
    return _addr;
}

mmap::operator const char *() const
{
    return _addr;
}


size_t mmap::size() const
{
    return _size;
}


}
}