cmake_minimum_required(VERSION 3.12)
project(tinyxml)
project(Lab2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES1 "./*.cpp")
file(GLOB SOURCES2 "./tinyxml/*.cpp")

add_library(tinyxml STATIC ${SOURCES2})
add_executable(Lab2 ${SOURCES1})

target_link_libraries(Lab2 PRIVATE tinyxml)
target_include_directories(Lab2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

