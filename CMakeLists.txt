cmake_minimum_required(VERSION 2.6)

# project name
project(fdcpp)

# add_definitions(-D_GNU_SOURCE)
add_definitions(-D_POSIX_SOURCE)
add_definitions(-Wall -Wextra)
add_definitions(-Wpedantic -Weffc++)
add_definitions(-O2 -DNDEBUG)
#add_definitions(-g3)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(TARGET fdcpp)
set(CMAKE_INSTALL_PREFIX /usr/local)
set(TARGET_SONAME lib${TARGET}.so)
set(TARGET_INSTALL_DIR lib)
set(HEADER_INSTALL_DIR include/fdcpp)

set(SRC
    src/fds/epoll.cpp
    src/fds/file_descriptor.cpp
    src/fds/file.cpp
    src/fds/signalfd.cpp
    src/fds/socket.cpp
    src/fds/timerfd.cpp
)

set(HDR
    src/fds/epoll.hpp
    src/fds/file_descriptor.hpp
    src/fds/file.hpp
    src/fds/signalfd.hpp
    src/fds/socket.hpp
    src/fds/timerfd.hpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

# add_library(${TARGET} SHARED ${SRC})

install(FILES
    ${HDR}
    DESTINATION
    ${HEADER_INSTALL_DIR}
)

install(FILES
    ${CMAKE_BINARY_DIR}/${TARGET_SONAME}
    DESTINATION
    ${TARGET_INSTALL_DIR}
)

add_subdirectory(tests/)