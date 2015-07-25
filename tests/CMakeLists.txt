cmake_minimum_required(VERSION 3.0.2)

# project name
project(tests)

find_package(Threads)

set(CMAKE_LIBRARY_PATH, /usr/local/lib)
set(LIBS fdcpp)
set(TARGETS
    epoll_test
    eventfd_test
    socket_test
    easy_timer_test
    timerfd_test
    signalfd_test
    easy_tcp_socket_test
    easy_unix_socket_test
    easy_buffered_socket_test
)

link_directories(${CMAKE_LIBRARY_PATH})

add_executable(epoll_test epoll_test.cpp)
target_link_libraries(epoll_test ${LIBS})

add_executable(eventfd_test eventfd_test.cpp)
target_link_libraries(eventfd_test ${LIBS})

add_executable(socket_test socket_test.cpp)
target_link_libraries(socket_test ${LIBS} ${CMAKE_THREAD_LIBS_INIT})

add_executable(easy_timer_test easy_timer_test.cpp)
target_link_libraries(easy_timer_test ${LIBS})

add_executable(timerfd_test timerfd_test.cpp)
target_link_libraries(timerfd_test ${LIBS})

add_executable(signalfd_test signalfd_test.cpp)
target_link_libraries(signalfd_test ${LIBS})

add_executable(easy_tcp_socket_test easy_tcp_socket_test.cpp)
target_link_libraries(easy_tcp_socket_test ${LIBS})

add_executable(easy_unix_socket_test easy_unix_socket_test.cpp)
target_link_libraries(easy_unix_socket_test ${LIBS})

add_executable(easy_buffered_socket_test easy_buffered_socket_test.cpp)
target_link_libraries(easy_buffered_socket_test ${LIBS} ${CMAKE_THREAD_LIBS_INIT})

add_custom_target(tests DEPENDS ${TARGETS})
set_target_properties(tests ${TARGETS} PROPERTIES EXCLUDE_FROM_ALL 1)