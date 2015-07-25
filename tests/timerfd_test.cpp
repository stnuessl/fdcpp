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

#include <fdcpp/fds/timerfd.hpp>

int main(int argc, char *argv[])
{
    struct itimerspec its;
    struct timespec sleep_time;
    
    (void) argc;
    (void) argv;
    
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 500 * 1e6;
    
    try {
        auto timer = fd::timerfd();
        timer.settime(its);
        
        nanosleep(&sleep_time, nullptr);
        
        timer.gettime(its);
        
        auto ns = its.it_value.tv_sec * 1e9 + its.it_value.tv_nsec;
        
        std::cout << "Remaining: " << ns / 1000 << " us\n";
        std::cout << "Timeouts: " << timer.read() << "\n";
        std::cout << "Ok\n";
        
    } catch (std::exception &e) {
        std::cerr << "** ERROR: " << e.what() << "\n";
    }
    
    return 0;
}