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

#include <string>
#include <cstring>

#include <system_error>
#include <stdexcept>

static std::string make_message(const char *tag, const char *func)
{
    auto t_len = strlen(tag);
    auto f_len = strlen(func);
    
    std::string msg;
    
    msg.reserve(t_len + f_len + sizeof("::"));
    
    msg += tag;
    
    if (*tag != '\0')
        msg += "::";
    
    msg += func;
    
    return msg;
}

void throw_system_error(const char *tag, const char *func, int e)
{
    auto what = make_message(tag, func);
    
    throw std::system_error(e, std::system_category(), what);
}

void throw_runtime_error(const char *tag, const char *func, const char *msg)
{
    auto what = make_message(tag, func);
    
    what += ": ";
    what += msg;
    
    throw std::runtime_error(what);
}
