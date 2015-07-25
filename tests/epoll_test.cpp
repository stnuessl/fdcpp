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
#include <string>
#include <stdexcept>
#include <unordered_map>

#include <fdcpp/fds/epoll.hpp>
#include <fdcpp/fds/eventfd.hpp>
#include <fdcpp/fds/inotify.hpp>
#include <fdcpp/fds/timerfd.hpp>
#include <fdcpp/fds/signalfd.hpp>
#include <fdcpp/easy/unix_socket.hpp>

#define MAX_EVENTS 10

bool loop = true;

void handle_inotify_event(const fd::inotify &inotify, 
                          std::unordered_map<int, std::string> &map)
{
    struct inotify_event *event;
    char buffer[4096];
    
    std::cout << "**INOTIFY: ";
    
    auto n = inotify.read(buffer, sizeof(buffer));
    
    FOR_EACH_INOTIFY_EVENT(buffer, n, event) {
        if (event->len > 0)
            std::cout << event->name << " ";
        else
            std::cout << map[event->wd] << " ";
        
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
    (void) timer.read();
    
    std::cout << "**TIMERFD: timeout\n";
}

void handle_signal(const fd::signalfd &sfd)
{
    struct signalfd_siginfo info;
    
    sfd.read(&info);
    
    std::cout << "**SIGNALFD: received " << strsignal(info.ssi_signo) << '\n';
    
    if (info.ssi_signo == SIGTERM || info.ssi_signo == SIGINT)
        loop = false;
}

void handle_socket(const fd::socket &sock)
{
    char buf[128];
    
    std::cout << "**UNIX_CONNECTION: received '";
    
    while (1) {
        auto n = sock.recv(buf, sizeof(buf), MSG_NOSIGNAL);
        if (n == 0)
            break;
            
        std::cout << std::string(buf, n);
    }
    
    std::cout << "'\n";
}

int main(int argc, char *argv[])
{
    struct itimerspec its;
    sigset_t mask;
    std::unordered_map<int, std::string> inotify_watch_map;
    struct epoll_event events[MAX_EVENTS], ev;
    
    /* setup inotify */
    
    auto inotify = fd::inotify();
    
    for (int i = 0; i < argc; ++i) {
        int wd = inotify.add_watch(argv[i], IN_OPEN | IN_MODIFY | IN_ACCESS);
        inotify_watch_map[wd] = std::string(argv[i]);
    }
    
    /* setup timerfd */

    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 2;
    its.it_interval.tv_nsec = 0;

    auto timer = fd::timerfd();
    timer.settime(its);
    
    /* setup signalfd */
    
    sigfillset(&mask);
    
    int err = sigprocmask(SIG_BLOCK, &mask, nullptr);
    if (err < 0)
        throw std::system_error(errno, std::system_category(), "sigprocmask()");
    
    auto sfd = fd::signalfd(mask);
    
    /* setup socket */
    unlink("/tmp/.epoll_test.sock");
    auto server = fd::easy::unix_socket::server("/tmp/.epoll_test.sock");
        
    /* setup epoll */
    
    auto epoll = fd::epoll();
    
    ev.events = EPOLLIN;
    ev.data.ptr = (void *) &inotify;
    epoll.ctl(EPOLL_CTL_ADD, inotify, ev);
    
    ev.data.ptr = (void *) &timer;
    epoll.ctl(EPOLL_CTL_ADD, timer, ev);
    
    ev.data.ptr = (void *) &sfd;
    epoll.ctl(EPOLL_CTL_ADD, sfd, ev);
    
    ev.data.ptr = (void *) &server;
    epoll.ctl(EPOLL_CTL_ADD, server, ev);
    
    std::cout << "Press CTRL + c to exit\n";
    
    while (loop) {
        auto nfds = epoll.wait(events, MAX_EVENTS);
        
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.ptr == (void *) &inotify) {
                handle_inotify_event(inotify, inotify_watch_map);
            } else if (events[i].data.ptr == (void *) &timer) {
                handle_timeout(timer);
            } else if (events[i].data.ptr == (void *) &sfd) {
                handle_signal(sfd);
            } else if (events[i].data.ptr == (void *) &server) {
                std::cout << "**SERVER SOCKET: new connection\n";
                auto sock = server.accept();
                
//                 int flags = sock.fcntl(F_GETFL);
//                 sock.fcntl(F_SETFL, flags | O_NONBLOCK);
                handle_socket(sock);
                
            }
        }
    }
    
    return 0;
}