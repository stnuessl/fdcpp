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

#ifndef _FD_UNIX_SOCKET_HPP
#define _FD_UNIX_SOCKET_HPP

#include <string>

#include <fds/socket.hpp>

namespace fd {

namespace easy {

class unix_socket : public socket {
public:
    explicit unix_socket();
    unix_socket(const unix_socket &other) = delete;
    unix_socket(unix_socket &&other);
    
    virtual ~unix_socket() = default;

    unix_socket &operator=(const unix_socket &other) = delete;
    unix_socket &operator=(unix_socket &&other);
    
    static unix_socket client(const char *path);
    static unix_socket client(const std::string &path);
    
    static unix_socket server(const char *path, int backlog = 5);
    static unix_socket server(const std::string &path, int backlog = 5);
};
    
}

}

#endif /* _FD_UNIX_SOCKET_HPP */