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

#define _DEFAULT_SOURCE
#include <endian.h>

#include <utility>
#include <system_error>
#include <cstring>
#include <cassert>

#include <util/throw.hpp>
#include <easy/buffered_socket.hpp>

namespace fd {
namespace easy {

buffered_socket::buffered_socket(socket &&socket, 
                                 size_t input_buffer_size, 
                                 size_t output_buffer_size)
    : _socket(std::move(socket)),
      _buffer_in(new char[input_buffer_size + output_buffer_size]),
      _in_rd(0),
      _in_size(0),
      _in_capacity(input_buffer_size),
      _buffer_out(_buffer_in + input_buffer_size),
      _out_size(0),
      _out_capacity(output_buffer_size)
{
}

buffered_socket::buffered_socket(buffered_socket &&other)
    : _socket(std::move(other._socket)),
      _buffer_in(other._buffer_in),
      _in_rd(other._in_rd),
      _in_size(other._in_size),
      _in_capacity(other._in_capacity),
      _buffer_out(other._buffer_out),
      _out_size(other._out_size),
      _out_capacity(other._out_capacity)
{
    other._buffer_in = nullptr;
}

buffered_socket &buffered_socket::operator=(buffered_socket &&other)
{
    _socket = std::move(other._socket);
    std::swap(_buffer_in, other._buffer_in);
    std::swap(_buffer_out, other._buffer_out);

    _in_rd = other._in_rd,
    _in_size = other._in_size;
    _in_capacity = other._in_capacity;
    _out_size = other._out_size;
    _out_capacity = other._out_capacity;
    
    return *this;
}

buffered_socket::~buffered_socket()
{
    if (_buffer_in)
        delete[] _buffer_in;
}

buffered_socket &buffered_socket::operator<<(char val)
{
    write_output_buffer(&val, sizeof(val));
    
    return *this;
}

buffered_socket &buffered_socket::operator<<(uint8_t val)
{
    write_output_buffer((char *) &val, sizeof(val));
    
    return *this;
}

buffered_socket &buffered_socket::operator<<(int8_t val)
{
    return operator<<(static_cast<uint8_t>(val));
}

buffered_socket &buffered_socket::operator<<(uint16_t val)
{
    val = htobe16(val);
    
    write_output_buffer((char *) &val, sizeof(val));

    return *this;
}

buffered_socket &buffered_socket::operator<<(int16_t val)
{
    return operator<<(static_cast<uint16_t>(val));
}

buffered_socket &buffered_socket::operator<<(uint32_t val)
{
    val = htobe32(val);
    
    write_output_buffer((char *) &val, sizeof(val));
    
    return *this;
}

buffered_socket &buffered_socket::operator<<(int32_t val)
{
    return operator<<(static_cast<uint32_t>(val));
}

buffered_socket &buffered_socket::operator<<(uint64_t val)
{
    val = htobe64(val);
    
    write_output_buffer((char *) &val, sizeof(val));

    return *this;
}

buffered_socket &buffered_socket::operator<<(int64_t val)
{
    return operator<<(static_cast<uint64_t>(val));
}

buffered_socket &buffered_socket::operator<<(float val)
{
    uint32_t ival;
    
    (void)memmove((char *) &ival, (char *) &val, sizeof(ival));
    
    return operator<<(ival);
}

buffered_socket &buffered_socket::operator<<(double val)
{
    uint64_t ival;
    
    (void)memmove((char *) &ival, (char *) &val, sizeof(ival));
    
    return operator<<(ival);
}

buffered_socket &buffered_socket::operator<<(const char *val)
{
    write_output_buffer(val, std::strlen(val) + 1);
    
    return *this;
}

buffered_socket &buffered_socket::operator<<(const std::string &val)
{
    write_output_buffer(val.c_str(), val.size() + 1);
    
    return *this;
}


buffered_socket &buffered_socket::operator>>(char &val)
{
    read_input_buffer(&val, sizeof(val));
    
    return *this;
}

buffered_socket &buffered_socket::operator>>(uint8_t &val)
{
    read_input_buffer((char *) &val, sizeof(val));

    return *this;
}

buffered_socket &buffered_socket::operator>>(int8_t &val)
{
    read_input_buffer((char *) &val, sizeof(val));
    
    return *this;
}

buffered_socket &buffered_socket::operator>>(uint16_t &val)
{
    read_input_buffer((char *) &val, sizeof(val));
    
    val = be16toh(val);
    
    return *this;
}

buffered_socket &buffered_socket::operator>>(int16_t &val)
{
    read_input_buffer((char *) &val, sizeof(val));
    
    val = be16toh(val);
    
    return *this;
}

buffered_socket &buffered_socket::operator>>(uint32_t &val)
{
    read_input_buffer((char *) &val, sizeof(val));
    
    val = be32toh(val);
    
    return *this;
}

buffered_socket &buffered_socket::operator>>(int32_t &val)
{
    read_input_buffer((char *) &val, sizeof(val));
    
    val = be32toh(val);
    
    return *this;
}

buffered_socket &buffered_socket::operator>>(uint64_t &val)
{
    read_input_buffer((char *) &val, sizeof(val));
    
    val = be64toh(val);
    
    return *this;
}

buffered_socket &buffered_socket::operator>>(int64_t &val)
{
    read_input_buffer((char *) &val, sizeof(val));
    
    val = be64toh(val);
    
    return *this;
}

buffered_socket &buffered_socket::operator>>(float &val)
{
    uint32_t ival;
    
    (void)operator>>(ival);
    
    memmove((char *) &val, &ival, sizeof(val));
    
    return *this;
}

buffered_socket &buffered_socket::operator>>(double &val)
{
    uint64_t ival;
    
    (void)operator>>(ival);
    
    memmove((char *) &val, (char *) &ival, sizeof(val));
    
    return *this;
}

buffered_socket &buffered_socket::operator>>(std::string &val)
{
    recv_if_possible();
    
    auto size = std::strlen(_buffer_in + _in_rd);

    val.clear();
    val.reserve(size);
    
    while (size--) {
        char c;
        
        read_input_buffer(&c, sizeof(c));
        
        val.push_back(c);
    }
    
    /* skip terminating '\0' */
    ++_in_rd;
    
    return *this;
}


void buffered_socket::buffered_socket::flush()
{
    send_all(_buffer_out, _out_size);
    _out_size = 0;
}

void buffered_socket::read_input_buffer(char *buffer, size_t size)
{
    recv_if_possible();
    
    if (_in_rd + size >= _in_size) {

        size_t copy_size  = _in_size - _in_rd;
        size_t recv_size = size - copy_size;

        memmove(buffer, _buffer_in + _in_rd, copy_size);
        if (recv_size != 0) {
            int flags = MSG_NOSIGNAL | MSG_WAITALL;
         
            _socket.recv(buffer + copy_size, recv_size, flags);
        }
        
        _in_rd = 0;
        _in_size = 0;
        
        return;
    }
    
    memmove(buffer, _buffer_in + _in_rd, size);
    _in_rd += size;
}

void buffered_socket::write_output_buffer(const char *buffer, size_t size)
{
    if (size >= _out_capacity) {
        send_all(buffer, size);
        return;
    }
    
    if (_out_size + size >= _out_capacity) {
        send_all(_buffer_out, _out_size);
        _out_size = 0;
    }
    
    (void)memmove(_buffer_out + _out_size, buffer, size);
    _out_size += size;
}


void buffered_socket::recv_if_possible() 
{
    if (_in_size >= _in_capacity)
        return;
    
    auto buffer = _buffer_in + _in_size;
    auto size = _in_capacity - _in_size;
    
    try {
        _in_size += _socket.recv(buffer, size, MSG_DONTWAIT | MSG_NOSIGNAL);
    } catch(std::system_error &e) {
        auto val = e.code().value();
        
        if (val != EAGAIN && val != EWOULDBLOCK)
            throw e;
    }
}

void buffered_socket::send_all(const char *buffer, size_t size) const
{
    size_t n = 0;
    
    do {
        n += _socket.send(buffer + n, size - n, MSG_NOSIGNAL);
    } while (n != size);
}


}
}