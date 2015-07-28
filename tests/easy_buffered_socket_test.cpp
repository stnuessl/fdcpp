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

#include <unistd.h>
#include <string.h>

#include <iostream>
#include <utility>
#include <chrono>
#include <thread>

#include <fdcpp/easy/unix_socket.hpp>
#include <fdcpp/easy/buffered_socket.hpp>
#include "util/macros.hpp"

struct message {
    explicit message()
        : c('X'), s(0x00ff), i(0xff0000ff), l(1), f(0.96f), d(0.2)
    {
    }
    
    bool operator==(const message &other)
    {
        return c == other.c &&
               s == other.s &&
               i == other.i &&
               l == other.l &&
               f == other.f &&
               d == other.d;
    }
    
    friend fd::easy::buffered_socket &operator<<(fd::easy::buffered_socket &stream, 
                                               const message &msg)
    {
        return stream << msg.c << msg.s << msg.i << msg.l << msg.f << msg.d;
    }
    
    friend fd::easy::buffered_socket &operator>>(fd::easy::buffered_socket &stream, 
                                               message &msg)
    {
        return stream >> msg.c >> msg.s >> msg.i >> msg.l >> msg.f >> msg.d;
    }

    char c;
    short s;
    int i;
    long l;
    float f;
    double d;
};

const int loop_count = 10000000;

void buffered_server(fd::easy::buffered_socket &stream)
{
    auto msg = message();
    
    for (int i = 0; i < loop_count; ++i)
        stream >> msg;
}

void buffered_client(fd::easy::buffered_socket &stream)
{
    auto msg = message();
    
    for (int i = 0; i < loop_count; ++i)
        stream << msg;
    
    stream.flush();
}

void unbuffered_server(fd::socket &socket)
{
    char buf[sizeof(message)];
    
    for (int i = 0; i < loop_count; ++i)
        socket.recv(buf, sizeof(buf));
}

void unbuffered_client(fd::socket &socket)
{
    char buf[sizeof(message)];
    
    memset(buf, 0xff, sizeof(buf));
    
    for (int i = 0; i < loop_count; ++i)
        socket.send(buf, sizeof(buf));
}

void test_buffered()
{
    unlink("/tmp/.socket_stream_test");
    
    auto server = fd::easy::unix_socket::server("/tmp/.socket_stream_test");
    auto client = fd::easy::unix_socket::client("/tmp/.socket_stream_test");
    
    auto conn = server.accept();
    
    auto server_stream = fd::easy::buffered_socket(std::move(conn));
    auto client_stream = fd::easy::buffered_socket(std::move(client));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::thread server_thread(buffered_server, std::ref(server_stream));
    std::thread client_thread(buffered_client, std::ref(client_stream));
    
    server_thread.join();
    client_thread.join();
    
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> diff = end - start;
    
    double throughput = loop_count * sizeof(message) / diff.count();
    
    std::cout << "---- Buffered ----\n" 
              << "Elapsed: " << diff.count() << " s\n"
              << "Throughput: " << throughput / 1e6 << " mb/s\n";
}


void test_unbuffered()
{
    unlink("/tmp/.socket_stream_test");
    
    auto server = fd::easy::unix_socket::server("/tmp/.socket_stream_test");
    auto client = fd::easy::unix_socket::client("/tmp/.socket_stream_test");
    
    auto conn = server.accept();
    
    auto start = std::chrono::high_resolution_clock::now();

    std::thread server_thread(unbuffered_server, std::ref(conn));
    std::thread client_thread(unbuffered_client, std::ref(client));
    
    server_thread.join();
    client_thread.join();
    
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> diff = end - start;
    
    double throughput = loop_count * sizeof(message) / diff.count();
    
    std::cout << "---- Unbuffered ----\n" 
              << "Elapsed: " << diff.count() << " s\n"
              << "Throughput: " << throughput / 1e6 << " mb/s\n";
}

void test_string()
{
    unlink("/tmp/.socket_stream_test");
    
    auto server = fd::easy::unix_socket::server("/tmp/.socket_stream_test");
    auto client = fd::easy::unix_socket::client("/tmp/.socket_stream_test");

    auto server_stream = fd::easy::buffered_socket(server.accept());
    auto client_stream = fd::easy::buffered_socket(std::move(client));
//     
    std::string msg1, msg2;
//     
    client_stream << "Hello, World!" << "This is a string message";
    client_stream.flush();
//     
    server_stream >> msg1 >> msg2;
    
    std::cout << "First message: \"" << msg1 << "\"\nSecond message: \""
              << msg2 << "\"\n";
    
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    test_buffered();
    test_unbuffered();
    test_string();
    
    return 0;
}