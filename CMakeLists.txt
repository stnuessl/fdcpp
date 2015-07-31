cmake_minimum_required(VERSION 3.0.2)

# project name
project(fdcpp)

# add_definitions(-D_GNU_SOURCE)
add_definitions(-D_POSIX_SOURCE)
add_definitions(-Wall -Wextra)
add_definitions(-std=c++11 -Wpedantic -Weffc++)
add_definitions(-O2 -DNDEBUG)
#add_definitions(-g3)

set(TARGET fdcpp)
set(CMAKE_INSTALL_PREFIX /usr/local)
set(TARGET_SONAME lib${TARGET}.so)
set(TARGET_INSTALL_DIR lib)
set(HEADER_INSTALL_DIR include/fdcpp)

set(SRC
    src/easy/tcp_socket.cpp
    src/easy/timer.cpp
    src/easy/udp_socket.cpp
    src/easy/unix_socket.cpp
    src/easy/buffered_socket.cpp
    src/easy/mmap.cpp
    src/easy/fdpass.cpp
    src/fds/base/descriptor.cpp
    src/fds/base/idescriptor.cpp
    src/fds/base/iodescriptor.cpp
    src/fds/epoll.cpp
    src/fds/eventfd.cpp
    src/fds/file.cpp
    src/fds/inotify.cpp
    src/fds/memfd.cpp
    src/fds/signalfd.cpp
    src/fds/socket.cpp
    src/fds/timerfd.cpp
    src/util/throw.cpp
)

set(HDR_FD_BASE
    src/fds/base/descriptor.hpp
    src/fds/base/idescriptor.hpp
    src/fds/base/iodescriptor.hpp
)

set(HDR_FD
    src/fds/epoll.hpp
    src/fds/eventfd.hpp
    src/fds/file.hpp
    src/fds/inotify.hpp
    src/fds/memfd.hpp
    src/fds/signalfd.hpp
    src/fds/socket.hpp
    src/fds/timerfd.hpp
)

set(HDR_EASY
    src/easy/tcp_socket.hpp
    src/easy/timer.hpp
    src/easy/udp_socket.hpp
    src/easy/unix_socket.hpp
    src/easy/buffered_socket.hpp
    src/easy/mmap.hpp
    src/easy/fdpass.hpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

add_library(${TARGET} SHARED ${SRC})

install(FILES
    ${HDR_FD_BASE}
    DESTINATION
    ${HEADER_INSTALL_DIR}/fds/base/
)

install(FILES
    ${HDR_FD}
    DESTINATION
    ${HEADER_INSTALL_DIR}/fds/
)

install(FILES
    ${HDR_EASY}
    DESTINATION
    ${HEADER_INSTALL_DIR}/easy/
)

install(FILES
    ${CMAKE_BINARY_DIR}/${TARGET_SONAME}
    DESTINATION
    ${TARGET_INSTALL_DIR}
)

add_subdirectory(tests/)