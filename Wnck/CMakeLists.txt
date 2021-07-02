cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME example_poject)
project(${PROJECT_NAME} CXX)
add_definitions(-std=c++17)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})

find_package(PkgConfig)
pkg_check_modules(WNCK REQUIRED libwnck-3.0)
include_directories(${WNCK_INCLUDE_DIRS})

message("include ${WNCK_INCLUDE_DIRS}")
message("library ${WNCK_LIBRARIES}")

find_package(nlohmann_json 3.2.0)
if(NOT ${nlohmann_json_FOUND})
    message("on ubuntu: apt-get install nlohmann-json3-dev")
endif()


set(LIB_NAME example)
add_executable(${LIB_NAME})
target_sources(${LIB_NAME} PRIVATE example.cpp)
target_link_libraries(${LIB_NAME} PUBLIC ${WNCK_LIBRARIES} nlohmann_json::nlohmann_json)



