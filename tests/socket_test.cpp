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

#include <iostream>
#include <thread>
#include <stdexcept>
#include <cstring>

#include <unistd.h>

#include <fds/socket.hpp>

#define SOCK_PATH "/tmp/.fd_socket_test.sock"
#define MSG_TEXT "Hello, World!"

void server_thread()
{
    struct sockaddr_un addr;
    char recv_buf[sizeof(MSG_TEXT)];
    
    unlink(SOCK_PATH);
    
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path));
    
    auto server = fd::socket(AF_UNIX, SOCK_STREAM);
    server.bind((const struct sockaddr *) &addr, sizeof(addr));
    server.listen();
    
    auto conn = server.accept();
    
    conn.recv(recv_buf, sizeof(recv_buf));
    
    if (strcmp(recv_buf, MSG_TEXT) != 0)
        throw std::logic_error("server: Invalid test result");
    
    conn.send(recv_buf, sizeof(recv_buf));
}

void client_thread()
{
    struct sockaddr_un addr;
    char recv_buf[sizeof(MSG_TEXT)];
    
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path));
    
    auto client = fd::socket(AF_UNIX, SOCK_STREAM);
    client.connect((const struct sockaddr *) &addr, sizeof(addr));
    client.write(MSG_TEXT, sizeof(MSG_TEXT));
    
    client.read(recv_buf, sizeof(recv_buf));
    
    if (strcmp(recv_buf, MSG_TEXT) != 0)
        throw std::logic_error("client: Invalid test result");
}

void test_unix_domain_socket()
{
    std::thread t1(&server_thread);
    std::thread t2(&client_thread);
    
    t1.join();
    t2.join();
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    test_unix_domain_socket();
    
    std::cout << "Ok\n";
    
    return 0;
}