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

udp_socket::udp_socket(int domain)
    : socket(domain, SOCK_DGRAM)
{
    if (domain != AF_INET && domain != AF_INET6)
        throw std::invalid_argument("udp_socket: udp_socket(): invalid domain");
}

udp_socket::udp_socket(udp_socket &&other)
    : socket(std::move(other))
{
}

udp_socket &udp_socket::operator=(udp_socket &&other)
{
    socket::operator=(std::move(other));
    
    return *this;
}

size_t udp_socket::recvfrom(char *buffer, 
                            size_t size, 
                            uint32_t addr, 
                            int flags) const
{

}


size_t udp_socket::recvfrom(char *buffer, 
                            size_t size, 
                            struct in6_addr *addr, 
                            int flags) const
{

}

size_t udp_socket::sendto(const char *buffer, 
                          size_t size, 
                          struct in6_addr *addr, 
                          int flags) const
{

}




}

}