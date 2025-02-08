cmake_minimum_required(VERSION 3.10)
project(TestIPP)

find_package(OpenCV REQUIRED)

add_executable(TestIPP main.cpp)
target_link_libraries(TestIPP ${OpenCV_LIBS})