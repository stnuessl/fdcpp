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