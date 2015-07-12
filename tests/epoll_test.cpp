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

#include <iostream>
#include <stdexcept>

#include <fdcpp/fds/epoll.hpp>
#include <fdcpp/fds/eventfd.hpp>
#include <fdcpp/fds/inotify.hpp>
#include <fdcpp/fds/timerfd.hpp>

#define MAX_EVENTS 10

bool loop = true;

void handle_stdin()
{
    std::string str;
    
    std::cout << "Stdin event:\n";
    
    if (std::getline(std::cin, str)) {
        if (str.empty())
            return;
        
        if (str == "exit" || str == "quit")
            loop = false;
        else
            std::cout << str << '\n';
    }
}

void handle_inotify_event(const fd::inotify &inotify)
{
    struct inotify_event *event;
    char buffer[4096];
    
    std::cout << "Inotify event:\n";
    
    auto n = inotify.read(buffer, 4096);
    
    FOR_EACH_INOTIFY_EVENT(buffer, n, event) {
        if (event->len > 0)
            std::cout << event->name << " ";
        if (event->mask & IN_ACCESS)
            std::cout << "IN_ACCESS ";
        if (event->mask & IN_MODIFY)
            std::cout << "IN_MODIFY ";
        if (event->mask & IN_OPEN)
            std::cout << "IN_OPEN ";
        
        std::cout << '\n';
    }
}

void handle_timeout(const fd::timerfd &timer)
{
    std::cout << "Timeouts since last read: " << timer.read() << '\n';
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    struct epoll_event events[MAX_EVENTS];
    
    auto timer = fd::timerfd();
    timer.settime({1000 * 1e6, 500 * 1e6});
    
    auto inotify = fd::inotify();
    inotify.add_watch(argv[0], IN_ACCESS | IN_MODIFY | IN_OPEN);
    
    auto epoll = fd::epoll();
    epoll.ctl(EPOLL_CTL_ADD, STDIN_FILENO, (void *) &std::cin);
    epoll.ctl(EPOLL_CTL_ADD, inotify, (void *) &inotify);
    epoll.ctl(EPOLL_CTL_ADD, timer, (void *) &timer);
    
    while (loop) {
        auto nfds = epoll.wait(events, MAX_EVENTS);
        
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.ptr == (void *) &std::cin)
                handle_stdin();
            else if (events[i].data.ptr == (void *) &inotify)
                handle_inotify_event(inotify);
            else if (events[i].data.ptr == (void *) &timer)
                handle_timeout(timer);
        }
    }
    
    std::cout << "Ok\n";
    
    return 0;
}