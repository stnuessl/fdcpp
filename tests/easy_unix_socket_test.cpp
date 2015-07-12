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

#include <string.h>
#include <unistd.h>

#include <fdcpp/easy/unix_socket.hpp>

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    char buffer_out[] = "Hello, World!";        
    char buffer_in[sizeof(buffer_out)];
    
    unlink("/tmp/.unix_sock_test");
    
    auto server = fd::easy::unix_socket::server("/tmp/.unix_sock_test");
    auto client = fd::easy::unix_socket::client("/tmp/.unix_sock_test");
    
    auto conn = server.accept();
        
    client.send(buffer_out, sizeof(buffer_out), MSG_NOSIGNAL);
    conn.recv(buffer_in, sizeof(buffer_in), MSG_NOSIGNAL);
    
    if (memcmp(buffer_in, buffer_out, sizeof(buffer_out)) != 0)
        throw std::logic_error("Invalid result");
        
    std::cout << "Ok\n";
    
    return 0;
}