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

#ifndef _FDCPP_FDPASS_HPP_
#define _FDCPP_FDPASS_HPP_

#include <initializer_list>
#include <string>
#include <vector>

#include <fds/base/descriptor.hpp>
#include <fds/socket.hpp>

namespace fd {
namespace easy {

class fdpass {
public:
    explicit fdpass(const char *path);
    explicit fdpass(const std::string &path);
    fdpass(fdpass &&other) = default;
    
    ~fdpass() = default;
    
    fdpass &operator=(fdpass &&other) = default;
    
    template<typename iter> 
    static void send(const socket &sock, iter begin, iter end);
    static void send(const socket &sock, 
                     const std::initializer_list<int> &list);
    static void send(const socket &sock, const descriptor &fd);
    
    template<typename iter> void send(iter begin, iter end);
    void send(const std::initializer_list<int> &list);
    void send(const descriptor &fd);
    
    static std::vector<descriptor> recv(const socket &sock);
    std::vector<descriptor> recv();
private:
    static constexpr long MAX_BATCH_SIZE = 10;
    
    socket _socket;
    std::string _path;
    bool _ready;
};



template<typename iter> 
void fdpass::send(const socket &sock, iter begin, iter end)
{
    struct msghdr msg;
    struct iovec iov;
    char buf[CMSG_SPACE(MAX_BATCH_SIZE * sizeof(int))];

    auto size = static_cast<size_t>(std::abs(std::distance(begin, end)));
    
    iov.iov_base = (void *) &size;
    iov.iov_len = sizeof(size);
    
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    msg.msg_name = nullptr;
    msg.msg_namelen = 0;
    
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    
    while (begin != end) {
        auto dist = std::abs(std::distance(begin, end));
        auto batch_size = std::min(static_cast<long>(dist), MAX_BATCH_SIZE);
        auto bytes = batch_size * sizeof(int);
        
        msg.msg_controllen = CMSG_SPACE(bytes);
        cmsg->cmsg_len = CMSG_LEN(bytes);
        
        int *ptr = (int *) CMSG_DATA(cmsg);
        
        while (batch_size--)
            *ptr++ = *begin++;
        
        sock.sendmsg(msg, MSG_NOSIGNAL);
    }
}

template<typename iter> void fdpass::send(iter begin, iter end)
{
    if (!_ready) {
        struct sockaddr_un addr;
        
        addr.sun_family = AF_UNIX;
        *stpncpy(addr.sun_path, _path.c_str(), sizeof(addr.sun_path)) = '\0';
        
        _socket.connect(addr);
        
        _ready = true;
    }
    
    fdpass::send(_socket, begin, end);
}

}
}



#endif /* _FDCPP_FDPASS_HPP_ */