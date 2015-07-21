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
#include <stdexcept>

#include <easy/udp_socket.hpp>

namespace fd {
namespace easy {
namespace udp_socket {

socket server(uint32_t addr, uint16_t port)
{
    struct sockaddr_in saddr;
    
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(addr);
    saddr.sin_port = htons(port);
    
    auto socket = fd::socket(AF_INET, SOCK_DGRAM);
    socket.bind(saddr);
    
    return socket;
}

socket server(const struct in6_addr &addr, uint16_t port)
{
    struct sockaddr_in6 saddr;
    
    saddr.sin6_family = AF_INET6;
    saddr.sin6_addr = addr;
    saddr.sin6_port = htons(port);
    saddr.sin6_flowinfo = 0;
    saddr.sin6_scope_id = 0;
    
    return server(saddr);
}


socket server(const struct sockaddr_in6 &saddr)
{
    auto socket = fd::socket(AF_INET6, SOCK_DGRAM);
    socket.bind(saddr);
    
    return socket;
}

}
}
}