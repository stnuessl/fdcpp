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

#include <easy/tcp_socket.hpp>

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    struct in6_addr addr6 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
    int msg_out = 0xDEADFACE;
    int msg_in;
    int on = 1;

    auto server = fd::easy::tcp_socket::server(&addr6, 5000);
    server.setsockopt(SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
    
    auto client = fd::easy::tcp_socket::client(&addr6, 5000);
    
    auto conn = server.accept();
    
    client.send((const char *) &msg_out, sizeof(msg_out));
    conn.recv((char *) &msg_in, sizeof(msg_in));
    
    if (msg_out != msg_in)
        throw std::logic_error("Invalid result");
    
    std::cout << "Ok\n";
    
    return 0;
}