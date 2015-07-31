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

#include <fdcpp/easy/fdpass.hpp>
#include <fdcpp/fds/memfd.hpp>
#include <fdcpp/fds/eventfd.hpp>

#include "util/macros.hpp"

static const char *path = "/tmp/.easy_fdpass_test.sock";
const int nfds = 1;

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

void test_multiple()
{
    auto pid = fork();
    ASSERT(pid >= 0, "fork() failed");
    
    if (pid > 0) {
        receiver_multiple();
        waitpid(0, nullptr, 0);
    } else {
        sender_multiple();
        exit(EXIT_SUCCESS);
    }
}

void receiver_single()
{
    auto v = fd::easy::fdpass(path).recv();
    
    ASSERT(v.size() == 1, "exspected only one descriptor");
}

void sender_single()
{
    auto memfd = fd::memfd("easy_fdpass_test");
    
    memfd.ftruncate(128);
    
    std::cout << "Sending: " << memfd << '\n';
    
    usleep(100 * 1000);   
    fd::easy::fdpass(path).send(memfd);
}

void test_single()
{
    auto pid = fork();
    ASSERT(pid >= 0, "fork() failed");
    
    if (pid > 0) {
        receiver_single();
        waitpid(0, nullptr, 0);
    } else {
        sender_single();
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    std::cout << "1\n";
    test_multiple();
    std::cout << "2\n";
    test_single();
    std::cout << "3\n";
    
    std::cout << "Ok\n";
    
    return 0;
}