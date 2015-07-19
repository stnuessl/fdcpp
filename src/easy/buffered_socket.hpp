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

#ifndef _FDCPP_BUFFERED_SOCKET_HPP_
#define _FDCPP_BUFFERED_SOCKET_HPP_

#include <string>

#include <fds/socket.hpp>

namespace fd {
namespace easy {

class buffered_socket {
public:
    explicit buffered_socket(socket &&socket, 
                           size_t input_buffer_size = 4096, 
                           size_t output_buffer_size = 4096);
    buffered_socket(const buffered_socket &other) = delete;
    buffered_socket(buffered_socket &&other);
    ~buffered_socket();
    
    buffered_socket &operator=(const buffered_socket &other) = delete;
    buffered_socket &operator=(buffered_socket &&other);
    
    buffered_socket &operator<<(char val);
    buffered_socket &operator<<(uint8_t val);
    buffered_socket &operator<<(int8_t val);
    buffered_socket &operator<<(uint16_t val);
    buffered_socket &operator<<(int16_t val);
    buffered_socket &operator<<(uint32_t val);
    buffered_socket &operator<<(int32_t val);
    buffered_socket &operator<<(uint64_t val);
    buffered_socket &operator<<(int64_t val);
    buffered_socket &operator<<(float val);
    buffered_socket &operator<<(double val);

    buffered_socket &operator>>(char &val);
    buffered_socket &operator>>(uint8_t &val);
    buffered_socket &operator>>(int8_t &val);
    buffered_socket &operator>>(uint16_t &val);
    buffered_socket &operator>>(int16_t &val);
    buffered_socket &operator>>(uint32_t &val);
    buffered_socket &operator>>(int32_t &val);
    buffered_socket &operator>>(uint64_t &val);
    buffered_socket &operator>>(int64_t &val);
    buffered_socket &operator>>(float &val);
    buffered_socket &operator>>(double &val);
    
    void flush();

private:
    void read_input_buffer(char *buffer, size_t size);
    void write_output_buffer(const char *buffer, size_t size);
    
    size_t recv(char *buffer, size_t size) const;
    void send(const char *buffer, size_t size) const;
    
    socket _socket;
    char *_buffer_in;
    size_t _in_rd;
    size_t _in_size;
    size_t _in_capacity;
    
    char *_buffer_out;
    size_t _out_size;
    size_t _out_capacity;
};

}
}

#endif /* _FDCPP_BUFFERED_SOCKET_HPP_ */