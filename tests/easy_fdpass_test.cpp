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
#include <sys/wait.h>

#include <iostream>
#include <vector>
#include <functional>

#include <fdcpp/easy/fdpass.hpp>
#include <fdcpp/easy/unix_socket.hpp>
#include <fdcpp/easy/mmap.hpp>
#include <fdcpp/fds/memfd.hpp>
#include <fdcpp/fds/eventfd.hpp>

#include "util/macros.hpp"

static const char *path = "/tmp/.easy_fdpass_test.sock";
const int nfds = 100;

void my_fork(std::function<void()> parent, std::function<void()> child)
{
    auto pid = fork();
    ASSERT(pid >= 0, "fork() failed");
    
    if (pid > 0) {
        parent();
        waitpid(0, nullptr, 0);
    } else {
        child();
        exit(EXIT_SUCCESS);
    }
}

void sender_multiple()
{
    std::vector<fd::eventfd> ev;
    
    for (int i = 0; i < nfds; ++i)
        ev.push_back(fd::eventfd(i + 1));

    usleep(100 * 1000);

    fd::easy::fdpass(path).send(ev.begin(), ev.end());
    
    for (const auto &x : ev)
        ASSERT(x >= 0, "descriptors should not be closed");
}

void receiver_multiple()
{
    auto v = fd::easy::fdpass(path).recv();
    
    ASSERT(v.size() == nfds, "invalid vector size");
    
    uint64_t count = 1;
    
    for (auto &x : v) {
        auto eventfd = fd::eventfd(std::move(x));
        
        ASSERT(count++ == eventfd.read(), "invalid eventfd counter value");
    }

    /* all fds where moved */
    for (auto &x : v)
        ASSERT(x < 0, "descriptors inside vector should have been moved");
}

void receiver_single()
{
    struct stat st;
    auto v = fd::easy::fdpass(path).recv();
    
    ASSERT(v.size() == 1, "exspected only one descriptor");
    
    auto memfd = fd::memfd(std::move(v[0]));
    memfd.fstat(st);
    
    auto mmap = fd::easy::mmap(st.st_size, PROT_READ, MAP_SHARED, memfd);
    
    ASSERT(strcmp(mmap, "Hello, World!") == 0, "invalid mmap content");
}

void sender_single()
{
    size_t size = 128;
    auto memfd = fd::memfd("easy_fdpass_test");
    memfd.ftruncate(size);
    
    auto mmap = fd::easy::mmap(size, PROT_WRITE, MAP_SHARED, memfd);
    
    strncpy(mmap, "Hello, World!", mmap.size());
    
    usleep(100 * 1000);
    fd::easy::fdpass(path).send(memfd);
}

void test_multiple()
{
    my_fork(&receiver_multiple, &sender_multiple);
}

void test_single()
{
    my_fork(&receiver_single, &sender_single);
}

void static_recv()
{
    unlink(path);
    
    auto sock = fd::easy::unix_socket::server(path).accept();
    
    auto v = fd::easy::fdpass::recv(sock);
    
    ASSERT(v.size() == nfds, "wrong vector size");
}

void static_send()
{
    std::vector<fd::eventfd> v;
    
    for (int i = 0; i < nfds; ++i)
        v.push_back(fd::eventfd());
    
    auto sock = fd::easy::unix_socket::client(path);
    
    fd::easy::fdpass::send(sock, v.begin(), v.end());
}

void test_static_usage()
{
    my_fork(&static_recv, &static_send);
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    test_multiple();
    test_single();
    test_static_usage();
    
    std::cout << "Ok\n";
    
    return 0;
}