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
#include <thread>
#include <cstring>

#include <unistd.h>

#include <fdcpp/fds/socket.hpp>

#include "util/macros.hpp"

#define SOCK_PATH "/tmp/.fd_socket_test.sock"
#define HELLO_WORLD "Hello, World!"

void server_thread()
{
    struct sockaddr_un addr;
    char recv_buf[sizeof(HELLO_WORLD)];
    
    unlink(SOCK_PATH);
    
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path));
    
    auto server = fd::socket(AF_UNIX, SOCK_STREAM);
    server.bind(addr);
    server.listen();
    
    auto conn = server.accept();
    
    conn.recv(recv_buf, sizeof(recv_buf));
    
    ASSERT(strcmp(recv_buf, HELLO_WORLD) == 0, "received wrong message");
    
    conn.send(recv_buf, sizeof(recv_buf));
}

void client_thread()
{
    struct sockaddr_un addr;
    char recv_buf[sizeof(HELLO_WORLD)];
    
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path));
    
    auto client = fd::socket(AF_UNIX, SOCK_STREAM);
    
    std::this_thread::yield();
    std::this_thread::yield();
    
    client.connect(addr);

    client.write(HELLO_WORLD, sizeof(HELLO_WORLD));
    
    client.read(recv_buf, sizeof(recv_buf));
    
    ASSERT(strcmp(recv_buf, HELLO_WORLD) == 0, "received wrong message");
}

void test_unix_domain_socket()
{
    std::thread t1(&server_thread);
    std::thread t2(&client_thread);
    
    t1.join();
    t2.join();
}

void test_udp_socket()
{
    char buffer_out[] = "Hello, World!";
    char buffer_in[sizeof(buffer_out)];
    size_t size = sizeof(buffer_out);
    
    auto client = fd::socket(AF_INET, SOCK_DGRAM);
    auto server = fd::socket(AF_INET, SOCK_DGRAM);
    
    struct sockaddr_in addr;
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(5000);
    
    server.bind(addr);
    client.sendto(buffer_out, size, addr);
    
    server.recvfrom(buffer_in, size);
    
    ASSERT(strcmp(buffer_in, buffer_out) == 0, "received wrong message");
}

void test_tcp_socket()
{
    struct sockaddr_in addr;
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(6000);
    
    try {
        auto socket = fd::socket(AF_INET, SOCK_STREAM);
        socket.bind(addr);
        socket.listen();
        
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        
        fd::socket(AF_INET, SOCK_STREAM).connect(addr);
        auto conn = socket.accept();
            
    } catch (std::system_error &e) {
        std::cerr << "** ERROR: socket initialization failed - " << e.what()
                  << '\n';
    }
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    test_unix_domain_socket();
    test_udp_socket();
    test_tcp_socket();
    
    std::cout << "Ok\n";
    
    return 0;
}