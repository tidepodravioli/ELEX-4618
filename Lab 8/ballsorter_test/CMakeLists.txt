cmake_minimum_required(VERSION 3.12)
project(Lab8)

set(OpenCV_STATIC ON)
set(OpenCV_RUNTIME vc16)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} "../../Common/cvui/" "../../Common/")
link_directories(${OpenCV_LIB_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.cpp")
add_executable(Lab8 ${SOURCES}) 

target_link_libraries(Lab8 PRIVATE ${OpenCV_LIBS})
target_include_directories(Lab8 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS})

