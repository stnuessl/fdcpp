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
#include <stdexcept>

#include <iostream>

#include <fdcpp/fds/signalfd.hpp>

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    sigset_t mask;
    struct signalfd_siginfo info;
    
    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTERM);
    
    sigprocmask(SIG_BLOCK, &mask, nullptr);
    
    kill(getpid(), SIGQUIT);
    kill(getpid(), SIGINT);
    kill(getpid(), SIGTERM);
    kill(getpid(), SIGHUP);
    
    auto sfd = fd::signalfd(mask, SFD_NONBLOCK);
    
    while (1) {
        try {
            sfd.read(info);
            
            std::cout << "Signal: " << strsignal(info.ssi_signo) << '\n';
        } catch (std::system_error &e) {
            if (e.code().value() == EAGAIN)
                break;
            
            throw e;
        }
    }
    
    std::cout << "Ok\n";
    
    return 0;
}