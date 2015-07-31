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

#ifndef _FDCPP_SOCKET_HPP_
#define _FDCPP_SOCKET_HPP_

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#include <string>
#include <cstdint>

#include <fds/base/iodescriptor.hpp>

namespace fd {

class socket : public iodescriptor {
public:    
    explicit socket(int domain, int type, int protocol = 0);
    explicit socket(descriptor &&other);
    socket(socket &&other) = default;
    
    virtual ~socket() = default;
    
    socket &operator=(socket &&other) = default;
    
    socket dup() const;
    void dup2(const socket &other) const;
    
    void connect(const struct sockaddr *saddr, socklen_t len) const;
    void connect(const struct sockaddr_un &saddr) const;
    void connect(const struct sockaddr_in &saddr) const;
    void connect(const struct sockaddr_in6 &saddr) const;
    
    void bind(const struct sockaddr *saddr, socklen_t len) const;
    void bind(const struct sockaddr_un &saddr) const;
    void bind(const struct sockaddr_in &saddr) const;
    void bind(const struct sockaddr_in6 &saddr) const;
    
    void listen(int backlog = 5) const;
    
    socket accept() const;
    socket accept(struct sockaddr *saddr, socklen_t *len) const;
    socket accept(struct sockaddr_storage *saddr, socklen_t *len) const; 
    
    void shutdown(int mode) const;
    
    size_t recv(char *buffer, size_t size, int flags = 0) const;
    size_t recvmsg(struct msghdr &msg, int flags = 0) const;
    size_t recvfrom(char *buffer, 
                    size_t size,
                    struct sockaddr *saddr,
                    socklen_t *len,
                    int flags = 0) const;
    size_t recvfrom(char *buffer, size_t size, int flags = 0) const;
    size_t recvfrom(char *buffer, 
                    size_t size, 
                    struct sockaddr_storage *saddr,
                    socklen_t *len,
                    int flags = 0) const;
    
    size_t send(const char *buffer, size_t size, int flags = 0) const;
    size_t sendmsg(const struct msghdr &msg, int flags = 0) const;
    size_t sendto(const char *buffer, 
                  size_t size, 
                  const struct sockaddr *saddr,
                  socklen_t len,
                  int flags = 0) const;
    size_t sendto(const char *buffer, 
                  size_t size, 
                  const struct sockaddr_un &saddr,
                  int flags = 0) const;
    size_t sendto(const char *buffer, 
                  size_t size, 
                  const struct sockaddr_in &saddr, 
                  int flags = 0) const;
    size_t sendto(const char *buffer, 
                  size_t size, 
                  const struct sockaddr_in6 &saddr, 
                  int flags = 0) const;
    
    bool sockatmark() const;
    
    void getsockopt(int level, int name, char *val, socklen_t *len) const;
    int getsockopt(int level, int name) const;
    
    void setsockopt(int level, int name, const char *val, socklen_t len) const;
    void setsockopt(int level, int name, int val) const;
    
    void getsockname(struct sockaddr *saddr, socklen_t *len) const;
    void getsockname(struct sockaddr_storage *saddr, socklen_t *len) const;
    
    void getpeername(struct sockaddr *saddr, socklen_t *len) const;
    void getpeername(struct sockaddr_storage *saddr, socklen_t *len) const;
private:
    explicit socket(int fd);
    socket(const socket &other);
    
    const socket &operator=(const socket &other) const;
};

}

#endif /* _FDCPP_SOCKET_HPP_ */