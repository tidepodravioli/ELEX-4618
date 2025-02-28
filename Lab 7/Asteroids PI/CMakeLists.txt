cmake_minimum_required(VERSION 3.12)
project(Lab7)

set(OpenCV_STATIC ON)
set(OpenCV_RUNTIME vc16)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} "../Common" "../Common/cvui" "./headers")
link_directories(${OpenCV_LIB_DIR})


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.cpp")
file(GLOB COMMON "../Common/*.cpp")
add_executable(Lab7 ${SOURCES} ${COMMON}) 

target_link_libraries(Lab7 PRIVATE ${OpenCV_LIBS})
target_include_directories(Lab7 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS})

