cmake_minimum_required(VERSION 2.6)

# project name
project(tests)

find_package(Threads)

add_executable(eventfd_test
    eventfd_test.cpp
    ../src/fds/base/file_descriptor.cpp
    ../src/fds/base/iofile_descriptor.cpp
    ../src/fds/file.cpp
    ../src/fds/eventfd.cpp
    ../src/util/throw.cpp
)

add_executable(socket_test
    socket_test.cpp
    ../src/fds/base/file_descriptor.cpp
    ../src/fds/base/iofile_descriptor.cpp
    ../src/fds/socket.cpp
    ../src/util/throw.cpp
)

target_link_libraries(socket_test ${CMAKE_THREAD_LIBS_INIT})

add_executable(epoll_test
    epoll_test.cpp
    ../src/fds/base/file_descriptor.cpp
    ../src/fds/base/iofile_descriptor.cpp
    ../src/fds/base/ifile_descriptor.cpp
    ../src/fds/epoll.cpp
    ../src/fds/inotify.cpp
    ../src/fds/timerfd.cpp
    ../src/util/throw.cpp
)

add_executable(easy_timer_test
    easy_timer_test.cpp
    ../src/easy/timer.cpp
    ../src/fds/base/file_descriptor.cpp
    ../src/fds/base/ifile_descriptor.cpp
    ../src/fds/timerfd.cpp
    ../src/util/throw.cpp
)

add_executable(timer_test
    timer_test.cpp
    ../src/fds/base/file_descriptor.cpp
    ../src/fds/base/ifile_descriptor.cpp
    ../src/fds/timerfd.cpp
    ../src/util/throw.cpp
)

add_executable(signalfd_test
    signalfd_test.cpp
    ../src/fds/signalfd.cpp
    ../src/fds/base/file_descriptor.cpp
    ../src/fds/base/ifile_descriptor.cpp
    ../src/util/throw.cpp
)

add_executable(easy_tcp_socket_test
    easy_tcp_socket_test.cpp
    ../src/easy/tcp_socket.cpp
    ../src/fds/socket.cpp
    ../src/fds/base/file_descriptor.cpp
    ../src/fds/base/iofile_descriptor.cpp
    ../src/util/throw.cpp
)

add_executable(easy_unix_socket_test
    easy_unix_socket_test.cpp
    ../src/easy/unix_socket.cpp
    ../src/fds/socket.cpp
    ../src/fds/base/file_descriptor.cpp
    ../src/fds/base/iofile_descriptor.cpp
    ../src/util/throw.cpp
)