cmake_minimum_required(VERSION 3.10)
project(cvui)

# Create an INTERFACE library (no compilation, only headers)
add_library(cvui INTERFACE)

# Add the include directory so other projects can find the headers
target_include_directories(cvui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})