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

#include <arpa/inet.h>

#include <fds/socket.hpp>

#include <util/throw.hpp>

static const char *tag = "socket";

namespace fd {

socket::socket(int domain, int type, int protocol)
    : iofile_descriptor(::socket(domain, type, protocol))
{
}

socket::socket(int fd)
    : iofile_descriptor(fd)
{
}


socket::socket(socket &&other)
    : iofile_descriptor(std::move(other))
{
}

socket::~socket()
{
}

socket &socket::operator=(socket&& other)
{
    iofile_descriptor::operator=(std::move(other));
    
    return *this;
}

void socket::connect(const sockaddr *saddr, socklen_t len) const
{
    auto err = ::connect(_fd, saddr, len);
    if (err < 0)
        throw_system_error(tag, "connect()");
}

void socket::connect(const struct sockaddr_un *saddr) const
{
    connect((const struct sockaddr *) saddr, sizeof(*saddr));
}

void socket::connect(const struct sockaddr_in *saddr) const
{
    connect((const struct sockaddr *) saddr, sizeof(*saddr));
}

void socket::connect(const struct sockaddr_in6 *saddr) const
{
    connect((const struct sockaddr *) saddr, sizeof(*saddr));
}

void socket::bind(const sockaddr *saddr, socklen_t len) const
{
    auto err = ::bind(_fd, saddr, len);
    if (err < 0)
        throw_system_error(tag, "bind()");
}

void socket::bind(const struct sockaddr_un *saddr) const
{
    bind((const struct sockaddr *) saddr, sizeof(*saddr));
}


void socket::bind(const struct sockaddr_in *saddr) const
{
    bind((const struct sockaddr *) saddr, sizeof(*saddr));
}


void socket::bind(const struct sockaddr_in6 *saddr) const
{
    bind((const struct sockaddr *) saddr, sizeof(*saddr));
}


void socket::listen(int backlog) const
{
    auto err = ::listen(_fd, backlog);
    if (err < 0)
        throw_system_error(tag, "listen()");
}

socket socket::accept() const
{
    return accept((struct sockaddr *) nullptr, nullptr);
}

socket socket::accept(struct sockaddr *saddr, socklen_t *len) const
{
    auto fd = ::accept(_fd, saddr, len);
    if (fd < 0)
        throw_system_error(tag, "accept()");
    
    return socket(fd);
}

socket socket::accept(struct sockaddr_storage *saddr, socklen_t *len) const
{
    return accept((struct sockaddr *) saddr, len);
}

void socket::shutdown(int mode) const
{
    auto err = ::shutdown(_fd, mode);
    if (err < 0)
        throw_system_error(tag, "shutdown()");
}

size_t socket::recv(char *buffer, size_t size, int flags) const
{
    auto n = ::recv(_fd, buffer, size, flags);
    if (n < 0)
        throw_system_error(tag, "recv()");
    
    return static_cast<size_t>(n);
}

size_t socket::recvmsg(struct msghdr *msg, int flags) const
{
    auto n = ::recvmsg(_fd, msg, flags);
    if (n < 0)
        throw_system_error(tag, "recvmsg()");
    
    return static_cast<size_t>(n);
}

size_t socket::recvfrom(char *buffer, 
                        size_t size, 
                        struct sockaddr *saddr, 
                        socklen_t *len, 
                        int flags) const
{
    auto n = ::recvfrom(_fd, buffer, size, flags, saddr, len);
    if (n < 0)
        throw_system_error(tag, "recvfrom()");
    
    return static_cast<size_t>(n);
}

size_t socket::recvfrom(char *buffer, size_t size, int flags) const
{
    return recvfrom(buffer, size, (struct sockaddr *) nullptr, nullptr, flags);
}

size_t socket::recvfrom(char *buffer, 
                        size_t size, 
                        struct sockaddr_storage *saddr, 
                        socklen_t *len, 
                        int flags) const
{
    return recvfrom(buffer, size, (struct sockaddr *) saddr, len, flags);
}


size_t socket::send(const char *buffer, size_t size, int flags) const
{
    auto n = ::send(_fd, buffer, size, flags);
    if (n < 0)
        throw_system_error(tag, "send()");
    
    return static_cast<size_t>(n);
}

size_t socket::sendmsg(const struct msghdr *msg, int flags) const
{
    auto n = ::sendmsg(_fd, msg, flags);
    if (n < 0)
        throw_system_error(tag, "sendmsg()");
    
    return static_cast<size_t>(n);
}

size_t socket::sendto(const char *buffer, 
                      size_t size, 
                      const struct sockaddr *saddr, 
                      socklen_t len, 
                      int flags) const
{
    auto n = ::sendto(_fd, buffer, size, flags, saddr, len);
    if (n < 0)
        throw_system_error(tag, "sendto()");
    
    return static_cast<size_t>(n);
}

size_t socket::sendto(const char *buffer, 
                      size_t size, 
                      const struct sockaddr_un *saddr, 
                      int flags) const
{
    const struct sockaddr *sockaddr = (const struct sockaddr *) saddr;
    
    return sendto(buffer, size, sockaddr, sizeof(*saddr), flags);
}

size_t socket::sendto(const char *buffer, 
                      size_t size,
                      const struct sockaddr_in *saddr,
                      int flags) const
{
    const struct sockaddr *sockaddr = (const struct sockaddr *) saddr;
    
    return sendto(buffer, size, sockaddr, sizeof(*saddr), flags);
}

size_t socket::sendto(const char *buffer, 
                      size_t size, 
                      const struct sockaddr_in6 *saddr, 
                      int flags) const
{
    const struct sockaddr *sockaddr = (const struct sockaddr *) saddr;
    
    return sendto(buffer, size, sockaddr, sizeof(*saddr), flags);
}


void socket::getsockopt(int level, int name, char *val, socklen_t *len) const
{
    auto err = ::getsockopt(_fd, level, name, val, len);
    if (err < 0)
        throw_system_error(tag, "getsockopt()");
}

int socket::getsockopt(int level, int name) const
{
    int ret;
    socklen_t len;
    
    len = sizeof(ret);
    
    getsockopt(level, name, (char *) &ret, &len);
    
    if (len != sizeof(ret)) {
        std::string msg = "invalid return value size '";
        msg += std::to_string(len);
        msg += "'";
        
        throw_runtime_error(tag, "getsockopt()", msg.c_str());
    }
    
    return ret;
}


void socket::setsockopt(int level, int name, const char *val, socklen_t len) const
{
    auto err = ::setsockopt(_fd, level, name, val, len);
    if (err < 0)
        throw_system_error(tag, "setsockopt()");
}

void socket::setsockopt(int level, int name, int val) const
{
    setsockopt(level, name, (char *) &val, sizeof(val));
}


void socket::getsockname(struct sockaddr *saddr, socklen_t *len) const
{
    auto err = ::getsockname(_fd, saddr, len);
    if (err < 0)
        throw_system_error(tag, "getsockname()");
}

void socket::getsockname(struct sockaddr_storage *saddr, socklen_t *len) const
{
    getsockname((struct sockaddr *) saddr, len);
}

void socket::getpeername(struct sockaddr *saddr, socklen_t *len) const
{
    auto err = ::getpeername(_fd, saddr, len);
    if (err < 0)
        throw_system_error(tag, "getpeername()");
}

void socket::getpeername(struct sockaddr_storage *saddr, socklen_t *len) const
{
    getpeername((struct sockaddr *) saddr, len);
}


}