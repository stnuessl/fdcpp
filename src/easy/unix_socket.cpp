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

#include <sys/un.h>
#include <string.h>

#include <utility>

#include <easy/unix_socket.hpp>

namespace fd {

namespace easy {

unix_socket::unix_socket()
    : socket(AF_UNIX, SOCK_STREAM)
{

}

unix_socket::unix_socket(fd::easy::unix_socket &&other)
    : socket(std::move(other))
{
}

unix_socket &unix_socket::operator=(unix_socket &&other)
{
    socket::operator=(std::move(other));
    
    return *this;
}

unix_socket unix_socket::client(const char *path)
{
    struct sockaddr_un addr;
    
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path));
    
    auto socket = unix_socket();
    socket.connect(addr);
    
    return socket;
}

unix_socket unix_socket::client(const std::string &path)
{
    return unix_socket::client(path.c_str());
}

unix_socket unix_socket::server(const char *path, int backlog)
{
    struct sockaddr_un addr;
    
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path));
    
    auto socket = unix_socket();
    
    socket.bind(addr);
    socket.listen(backlog);
    
    return socket;
}

unix_socket unix_socket::server(const std::string &path, int backlog)
{
    return unix_socket::server(path.c_str(), backlog);
}


}
}