cmake_minimum_required(VERSION 3.12)
project(Lab7CControl)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_library(PIGPIO_LIB NAMES pigpio REQUIRED)
find_library(PIGPIOD_IF2_LIB NAMES pigpiod_if2 REQUIRED)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB SOURCES "*.cpp")
add_executable(Lab7CControl ${SOURCES}) 

target_link_libraries(Lab7CControl PRIVATE ${PIGPIO_LIB} ${PIGPIO_IF2_LIB} ${CURSES_LIBRARIES})
target_include_directories(Lab7CControl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set(THREADS_PREFER_THREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Lab7CControl PRIVATE Threads::Threads)