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

// #include <string.h>

#include <utility>
#include <stdexcept>

#include <easy/tcp_socket.hpp>

namespace fd {
namespace easy {

tcp_socket::tcp_socket(int domain)
    : socket(domain, SOCK_STREAM)
{
    if (domain != AF_INET && domain != AF_INET6)
        throw std::invalid_argument("tcp_socket: tcp_socket(): invalid domain");
}

tcp_socket::tcp_socket(tcp_socket &&other)
    : socket(std::move(other))
{
}

tcp_socket &tcp_socket::operator=(tcp_socket &&other)
{
    socket::operator=(std::move(other));
    
    return *this;
}


tcp_socket tcp_socket::client(uint32_t addr, uint16_t port)
{
    struct sockaddr_in saddr;
    
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(addr);
    saddr.sin_port = htons(port);
    
    auto socket = tcp_socket(AF_INET);
    socket.connect(saddr);
    
    return socket;
}

tcp_socket tcp_socket::client(const struct sockaddr_in6 &saddr)
{
    auto socket = tcp_socket(AF_INET6);
    socket.connect(saddr);
    
    return socket;
}


tcp_socket tcp_socket::client(const struct in6_addr &addr, uint16_t port)
{
    struct sockaddr_in6 saddr;
    
    saddr.sin6_family = AF_INET6;
    saddr.sin6_addr = addr;
    saddr.sin6_port = htons(port);
    saddr.sin6_flowinfo = 0;
    saddr.sin6_scope_id = 0;
    
    return client(saddr);
}


tcp_socket tcp_socket::server(uint32_t addr, uint16_t port, int backlog)
{
    struct sockaddr_in saddr;
    
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(addr);
    saddr.sin_port = htons(port);
    
    auto socket = tcp_socket(AF_INET);
    socket.bind(saddr);
    socket.listen(backlog);
    
    return socket;
}

tcp_socket tcp_socket::server(const struct sockaddr_in6 &saddr, int backlog)
{
    auto socket = tcp_socket(AF_INET6);
    socket.bind(saddr);
    socket.listen(backlog);
    
    return socket;
}

    
tcp_socket tcp_socket::server(const struct in6_addr &addr, 
                              uint16_t port, 
                              int backlog)
{
    struct sockaddr_in6 saddr;
    
    saddr.sin6_family = AF_INET6;
    saddr.sin6_addr = addr;
    saddr.sin6_port = htons(port);
    saddr.sin6_flowinfo = 0;
    saddr.sin6_scope_id = 0;
    
    return server(saddr, backlog);
}




}
}