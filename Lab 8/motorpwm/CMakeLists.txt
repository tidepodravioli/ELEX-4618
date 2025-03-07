cmake_minimum_required(VERSION 3.12)
project(motorpwm)

set(OpenCV_STATIC ON)
set(OpenCV_RUNTIME vc16)
find_package(OpenCV REQUIRED)

find_library(PIGPIO_LIB NAMES pigpio REQUIRED)
find_library(PIGPIOD_IF2_LIB NAMES pigpiod_if2 REQUIRED)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

include_directories(${OpenCV_INCLUDE_DIRS} "../../Common" "../../Common/cvui" "./headers")
link_directories(${OpenCV_LIB_DIR})


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.cpp")
#file(GLOB COMMON "../../Common/*.cpp")
add_executable(motorpwm ${SOURCES}) 

target_link_libraries(motorpwm PRIVATE ${OpenCV_LIBS} ${PIGPIO_LIB} ${PIGPIO_IF2_LIB} ${CURSES_LIBRARIES})
target_include_directories(motorpwm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS})

set(THREADS_PREFER_THREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(motorpwm PRIVATE Threads::Threads)