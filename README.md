# fdcpp - file descriptors for C++

Guess what... 

    TODO
    
## Advantages

* Nice C++ interface to write your own abstractions based on the library classes.
* No boilerplate code for error checking: errors are propagated via exceptions.
* Overloaded functions and default arguments to simplify interfaces.
* File descriptor leakage difficult. Objects are not copyable.

## Disadvantages

* C header files leak into your sources via library includes.

## TCP socket example
### C Version

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
    
    while (1) {
        int new_sock = accept(sock, NULL, NULL);
        if (new_sock < 0) {
            perror("accept");
            continue;
        }
        
        /* handle connection */
    }
    
    exit(EXIT_SUCCESS);
    
fail:
    close(sock);
    exit(EXIT_FAILURE);
}
```

### C++ fdcpp Version

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
        
        while (1) {
            auto conn = socket.accept();
            
            /* handle connection */
        }
    } catch (std::system_error &e) {
        std::cerr << "** ERROR: socket initialization failed - " << e.what();
    }
}
```