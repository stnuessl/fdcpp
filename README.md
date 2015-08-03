# fdcpp - file descriptors for C++

README not finished yet.

## Overview

* [Dependencies](https://github.com/stnuessl/fdcpp#dependencies)
* [Installation](https://github.com/stnuessl/fdcpp#installation)
* [Deinstallation](https://github.com/stnuessl/fdcpp#deinstallation)
* [Advantages](https://github.com/stnuessl/fdcpp#advantages)
* [Disadvantages](https://github.com/stnuessl/fdcpp#disadvantages)
* [Documentation](https://github.com/stnuessl/fdcpp#documentation)
* [Implemented classes](https://github.com/stnuessl/fdcpp#implemented-classes)
    * [epoll](https://github.com/stnuessl/fdcpp#epoll)
    * [eventfd](https://github.com/stnuessl/fdcpp#eventfd)
    * [file](https://github.com/stnuessl/fdcpp#file)
    * [inotify](https://github.com/stnuessl/fdcpp#inotify)
    * [signalfd](https://github.com/stnuessl/fdcpp#signalfd)
    * [socket](https://github.com/stnuessl/fdcpp#socket)
    * [timerfd](https://github.com/stnuessl/fdcpp#timerfd)
* [TCP socket example](https://github.com/stnuessl/fdcpp#tcp-socket-example)
    * [C Version](https://github.com/stnuessl/fdcpp#c-version)
    * [C++ fdcpp version](https://github.com/stnuessl/fdcpp#c-fdcpp-version)
    * [C++ fdcpp easy version](https://github.com/stnuessl/fdcpp#c-fdcpp-easy-version)

## Dependencies

## Installation

    $ git clone https://github.com/stnuessl/fdcpp
    $ mkdir fdcpp/build
    $ cd fdcpp/build
    $ cmake ../
    $ make
    # make install
    
## Deinstallation

    # rm -rf /usr/local/include/fdcpp /usr/local/lib/libfdcpp.so

## Advantages

* Nice C++ interface to write your own abstractions based on the library classes.
* No boilerplate code for error checking: errors are propagated via exceptions.
* Overloaded functions and default arguments to simplify interfaces.
* No file descriptor leaking.
* The use of class functions makes it impossible to call an inappropriate function
on a file descriptor (e.g. calling accept() on an opened file).

## Disadvantages

* GNU/Linux only
* C header files leak into your sources via library includes.
* Not all functions for file descriptors are implemented.

## Documentation

All class function names have the same name as their corresponding system calls.
This makes the library easy to use if you are already familiar with those interfaces and
also makes an extra documentation for this library unnecassary. You can just type
'man <class function name>' into your terminal (or search for it on the web) and
have the documentation right there. Notice that special file descriptor functions like 
epoll_ctl() and inotify_add_watch() degenerate to ctl() and add_watch() respectively.
This means you can use the class like this:
    
    auto epoll = fd::epoll();
    epoll.ctl(...);
    
instead of

    auto epoll = fd::epoll();
    epoll.epoll_ctl(...);

## Implemented classes

### epoll
### eventfd
### file
### inotify
### signalfd
### socket
### timerfd

## TCP socket example

Each of the following simple code snippets set up a TCP server socket using 
either plain C or the fdcpp library.

### C version

```c
#include <stdio.h>              // perror()
#include <unistd.h>             // close()
#include <stdlib.h>             // exit(): not really needed though
#include <sys/socket.h>         // socket(), bind(), listen(), accept()
#include <netinet/in.h>         // struct sockaddr_in

{
    struct sockaddr_in addr;
    int sock, err;
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(5000);
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        goto fail;
    }
    
    err = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0) {
        perror("bind");
        goto fail;
    }
    
    err = listen(sock, 5);
    if (err < 0) {
        perror("listen");
        goto fail;
    }

    int new_sock = accept(sock, NULL, NULL);
    if (new_sock < 0) {
        perror("accept");
        goto fail;
    }
    
    /* handle connection here */
    
    close(new_sock);
    close(sock);
    
    exit(EXIT_SUCCESS);
    
fail:
    if (sock >= 0)
        close(sock);
    
    exit(EXIT_FAILURE);
}
```

### C++ fdcpp version

```cpp
#include <iostream>
#include <fdcpp/fds/socket.hpp>

{
    struct sockaddr_in addr;
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(6000);
    
    try {
        auto socket = fd::socket(AF_INET, SOCK_STREAM);
        socket.bind(addr);
        socket.listen();        // = socket.listen(5)
        
        auto conn = socket.accept();
            
        /* handle connection here */
    } catch (std::system_error &e) {
        std::cerr << "** ERROR: socket initialization failed - " << e.what()
                  << '\n';
    }
}
```

### C++ fdcpp easy version

```cpp
#include <iostream>
#include <fdcpp/easy/tcp_socket.hpp>

{
    try {
        auto conn = fd::easy::tcp_socket::server(INADDR_ANY, 7000).accept();
        
        /* handle connection here */
    } catch (std::system_error &e) {
        std::cerr << "** ERROR: socket initialization failed - " << e.what()
                  << '\n';
    }
}
```

## descriptor passing

The following code snippets show an example of descriptor passing via unix
domain sockets. The first two snippets show the sender and receiver code in C.
The last two snippets show the corresponding code in C++ with fdcpp library.

### C version

#### sender

```c
#include <unistd.h>         /* close(), read() */
#include <stdlib.h>         /* exit() */
#include <stdio.h>          /* perror() */
#include <string.h>         /* stpncpy() */
#include <sys/timerfd.h>    /* timerfd_create(), timerfd_settime() */
#include <sys/socket.h>     /* socket(), connect(), bind(), etc. */
#include <sys/un.h>         /* sockaddr_un */
#include <time.h>           /* CLOCK_MONOTONIC */
{
    struct iovec iov;
    struct msghdr msg;
    struct cmsghdr *cmsg;
    struct itimerspec its;
    struct sockaddr_un addr;
    char buf[CMSG_SPACE(sizeof(int))];
    int tfd = -1, sock = -1, err;
    
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 100 * 1e3;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 100 * 1e3;
    
    tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tfd < 0) {
        perror("eventfd_create");
        goto fail;
    }
    
    err = timerfd_settime(tfd, 0, &its, NULL);
    if (err < 0) {
        perror("eventfd_settime");
        goto fail;
    }
    
    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        goto fail;
    }
    
    addr.sun_family = AF_UNIX;
    *stpncpy(addr.sun_path, "/tmp/.unique.sock", sizeof(addr.sun_path)) = '\0';
    
    err = connect(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0) {
        perror("connect");
        goto fail;
    }
    
    iov.iov_base = buf;
    iov.iov_len = sizeof(buf);
    
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    
    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    *(int *) CMSG_DATA(cmsg) = tfd;
    
    err = sendmsg(sock, &msg, MSG_NOSIGNAL);
    if (err < 0) {
        perror("sendmsg");
        goto fail;
    }
    
    close(tfd);
    close(sock);
    
    return;
fail:
    if (tfd >= 0)
        close(tfd);
    if (sock >= 0)
        close(sock);
        
    exit(EXIT_FAILURE);
}
```

#### receiver

```c
#include <unistd.h>         /* close(), read() */
#include <stdlib.h>         /* exit() */
#include <stdio.h>          /* perror() */
#include <string.h>         /* stpncpy() */
#include <sys/timerfd.h>    /* timerfd_create(), timerfd_settime() */
#include <sys/socket.h>     /* socket(), connect(), bind(), etc. */
#include <sys/un.h>         /* sockaddr_un */
#include <time.h>           /* CLOCK_MONOTONIC */
#include <stdint.h>         /* uint64_t */
{
    struct iovec iov;
    struct msghdr msg;
    struct cmsghdr *cmsg;
    struct sockaddr_un addr;
    char buf[CMSG_SPACE(sizeof(int))];
    int tfd = -1, sock = -1, count, err;
    ssize_t n;
    uint64_t val;
    
    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        goto fail;
    }
    
    addr.sun_family = AF_UNIX;
    *stpncpy(addr.sun_path, "/tmp/.unique.sock", sizeof(addr.sun_path)) = '\0';
    
    unlink(addr.sun_path);
    err = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (err < 0) {
        perror("bind");
        goto fail;
    }
    
    iov.iov_base = buf;
    iov.iov_len = sizeof(buf);
    
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    
    err = recvmsg(sock, &msg, MSG_NOSIGNAL);
    if (err < 0) {
        perror("recvmsg");
        goto fail;
    }
    
    tfd = *(int *) CMSG_DATA(CMSG_FIRSTHDR(&msg));
    
    count = 1000;
    
    while (count > 0) {
        n = read(tfd, &val, sizeof(val));
        if (n < 0) {
            perror("read");
            goto fail;
        }
        
        count -= val;
    }
    
    close(tfd);
    close(sock);
    
    return;
fail:
    if (sock >= 0)
        close(sock);
    if (tfd >= 0)
        close(tfd);
    
    exit(EXIT_FAILURE);
}
```

### C++ fdcpp easy version

#### sender

```cpp
#include <iostream>
#include <fdcpp/easy/fdpass.hpp>
#include <fdcpp/fds/timerfd.hpp>
{
    struct itimerspec its;
    
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 100 * 1e3;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 100 * 1e3;
    
    try {
        auto tfd = fd::timerfd();
        tfd.settime(its);
        
        fd::easy::fdpass("/tmp/.unique.sock").send(tfd);
    } catch (std::system_error &e) {
        std::cerr << "** ERROR: " << e.what() << '\n';
    }
}
```

#### receiver

```cpp
#include <iostream>
#include <fdcpp/easy/fdpass.hpp>
#include <fdcpp/fds/timerfd.hpp>
{
    try {
        auto v = fd::easy::fdpass("/tmp/.unique.sock").recv();
        
        auto tfd = fd::timerfd(std::move(v[0]));
        
        int count = 1000;
        
        while (count > 0)
            count -= tfd.read();
    } catch (std::system_error &e) {
        std::cerr << "** ERROR: " << e.what() << '\n';
    }
}
```