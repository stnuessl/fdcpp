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

#include <algorithm>

#include <easy/fdpass.hpp>

namespace fd {

namespace easy {

fdpass::fdpass(const char *path)
    : _socket(AF_UNIX, SOCK_DGRAM),
      _path(path),
      _ready(false)
{
}

fdpass::fdpass(const std::string &path)
    : fdpass(path.c_str())
{
}

void fdpass::send(const socket &sock, const descriptor &fd)
{
    std::vector<int> v = { fd };
    
    fdpass::send(sock, v.begin(), v.end());
}

void fdpass::send(const descriptor &fd)
{
    std::vector<int> v = { fd };

    send(v.begin(), v.end());
}

std::vector< descriptor > fdpass::recv(const socket &sock)
{
    std::vector<descriptor> v;
    struct msghdr msg;
    struct cmsghdr *cmsg;
    struct iovec iov;
    size_t size;
    char buf[CMSG_SPACE(MAX_BATCH_SIZE * sizeof(int))];
    
    iov.iov_base = (void *) &size;
    iov.iov_len = sizeof(size);
    
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    msg.msg_name = nullptr;
    msg.msg_namelen = 0;
    
    do {
        sock.recvmsg(msg, MSG_NOSIGNAL);
        
        long diff = size - v.size();
        auto batch_size = std::min(diff, MAX_BATCH_SIZE);
        
        cmsg = CMSG_FIRSTHDR(&msg);
        int *ptr = (int *) CMSG_DATA(cmsg);
        
        while (batch_size--)
            v.push_back(descriptor(*ptr++));
        
    } while(v.size() != size);
    
    return v;
}

std::vector<descriptor> fdpass::recv()
{
    if (!_ready) {
        struct sockaddr_un addr;
        
        addr.sun_family = AF_UNIX;
        *stpncpy(addr.sun_path, _path.c_str(), sizeof(addr.sun_path)) = '\0';
        
        unlink(addr.sun_path);
        _socket.bind(addr);
        
        _ready = true;
    }
    
    return fdpass::recv(_socket);
}


}
}