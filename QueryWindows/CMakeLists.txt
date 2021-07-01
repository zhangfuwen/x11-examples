cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME example_poject)
project(${PROJECT_NAME} CXX)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})

set(LIB_NAME example)
add_executable(${LIB_NAME})
target_sources(${LIB_NAME} PRIVATE example.cpp)



