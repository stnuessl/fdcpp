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

#ifndef _FD_INOTIFY_HPP
#define _FD_INOTIFY_HPP

#include <sys/inotify.h>
#include <string>

#include <fds/base/ifile_descriptor.hpp>

#define FOR_EACH_INOTIFY_EVENT(__buffer, __size, __event)                      \
    for ((__event) = (struct inotify_event *)  (__buffer);                     \
         (char *)(__event) < (char *) (__buffer) + (__size);                   \
         (__event) = (struct inotify_event *)                                  \
         ((char *)(__event) + sizeof(*(__event)) + (__event)->len))

namespace fd {

class inotify : public ifile_descriptor {
public:
    explicit inotify(int flags = 0);
    inotify(inotify &&other);
    
    virtual ~inotify() = default;
    
    inotify dup() const;
    void dup2(const inotify &other) const;
    
    inotify &operator=(inotify &&other);
    
    int add_watch(const char *path, uint32_t mask) const;
    int add_watch(const std::string &path, uint32_t mask) const; 
    void rm_watch(int wd) const;
private:
    inotify(const inotify &other);
    const inotify &operator=(const inotify &other) const;
};

}

#endif /* _FD_INOTIFY_HPP */