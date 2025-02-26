cmake_minimum_required(VERSION 3.12)
project(rpitest)

find_library(PIGPIO_LIB NAMES pigpio REQUIRED)
find_library(PIGPIOD_IF2_LIB NAMES pigpiod_if2 REQUIRED)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(rpitest ./main.cpp)
target_link_libraries(rpitest PRIVATE ${PIGPIO_LIB} ${PIGPIO_IF2_LIB} ${CURSES_LIBRARIES})

set(THREADS_PREFER_THREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(rpitest PRIVATE Threads::Threads)
