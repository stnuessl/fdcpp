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

#include <iostream>
#include <stdexcept>

#include <netinet/in.h>

#include <fdcpp/easy/tcp_socket.hpp>

#include "util/macros.hpp"

void ipv4()
{
    int msg_out = 0xDEADFACE;
    int msg_in;
    
    auto server = fd::easy::tcp_socket::server(INADDR_LOOPBACK, 5000);
    auto client = fd::easy::tcp_socket::client(INADDR_LOOPBACK, 5000);
    auto conn = server.accept();
        
    client.send((char *) &msg_out, sizeof(msg_out));
    conn.recv((char *) &msg_in, sizeof(msg_in));
    
    ASSERT(msg_in == msg_out);
}

void ipv6()
{
    int msg_out = 0xDEADFACE;
    int msg_in;
    
    struct sockaddr_in6 addr;
    
    addr.sin6_family = AF_INET6;
    addr.sin6_addr = in6addr_any;
    addr.sin6_port = htons(5001);
    addr.sin6_flowinfo = 0;
    addr.sin6_scope_id = 0;
    
    auto server = fd::easy::tcp_socket::server(&addr);
    
    addr.sin6_addr = in6addr_loopback;
    
    auto client = fd::easy::tcp_socket::client(&addr);
    auto conn = server.accept();
    
    client.send((char *) &msg_out, sizeof(msg_out));
    conn.recv((char *) &msg_in, sizeof(msg_in));
    
    ASSERT(msg_in == msg_out);
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    ipv4();
    ipv6();

    std::cout << "Ok\n";
    
    return 0;
}