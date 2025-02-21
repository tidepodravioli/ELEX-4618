cmake_minimum_required(VERSION 3.12)
project(joysticktest)

set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} "../Common" "../Common/cvui")
link_directories(${OpenCV_LIB_DIR} "../Common/cvui")


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.cpp")
file(GLOB COMMON "../Common/*.cpp")
add_executable(joysticktest ${SOURCES} ${COMMON}) 

target_link_libraries(joysticktest PRIVATE ${OpenCV_LIBS})
target_include_directories(joysticktest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS})

