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

#ifndef _FD_TCP_SOCKET_HPP_
#define _FD_TCP_SOCKET_HPP_

#include <netinet/in.h>

#include <fds/socket.hpp>

namespace fd {
namespace easy {
namespace tcp_socket {

socket client(uint32_t addr, uint16_t port);
socket client(const struct sockaddr_in6 &saddr);
socket client(const struct in6_addr &addr, uint16_t port);

socket server(uint32_t addr, uint16_t port, int backlog = 5);
socket server(const struct sockaddr_in6 &saddr, int backlog = 5);
socket server(const struct in6_addr &addr, uint16_t port, int backlog = 5);

}
}
}

#endif /* _FD_TCP_SOCKET_HPP_ */