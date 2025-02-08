cmake_minimum_required(VERSION 3.12)
project(Lab3)

set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#file(GLOB SOURCES "*.cpp")
add_executable(Lab3 main.cpp CControl.cpp Serial.cpp getstuff.cpp IOTest.cpp CJoystickPosition.cpp) 

target_link_libraries(Lab3 ${OpenCV_LIBS} "C:\\lib\\opencv\\opencv\\x64\\vc17\\staticlib\\ippicvmt.lib")
target_include_directories(Lab3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})