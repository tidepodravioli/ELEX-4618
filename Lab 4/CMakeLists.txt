cmake_minimum_required(VERSION 3.12)
project(Lab4)

set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} "C:/Users/alecb/repo/OOP/Lab 3")
link_directories(${OpenCV_LIB_DIR})

add_subdirectory("C:/Users/alecb/repo/OOP/Lab 4/cvui")


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.cpp")
add_executable(Lab4
${SOURCES}
"C:/Users/alecb/repo/OOP/Lab 3/CControl.cpp"
"C:/Users/alecb/repo/OOP/Lab 3/Serial.cpp"
"C:/Users/alecb/repo/OOP/Lab 3/getstuff.cpp" 
"C:/Users/alecb/repo/OOP/Lab 3/CJoystickPosition.cpp") 

target_link_libraries(Lab4 PRIVATE ${OpenCV_LIBS} cvui)
target_include_directories(Lab4 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS})

