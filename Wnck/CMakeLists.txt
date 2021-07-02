cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME example_poject)
project(${PROJECT_NAME} CXX)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})

find_package(PkgConfig)
pkg_check_modules(WNCK REQUIRED libwnck-3.0)
include_directories(${WNCK_INCLUDE_DIRS})

message("include ${WNCK_INCLUDE_DIRS}")
message("library ${WNCK_LIBRARIES}")



set(LIB_NAME example)
add_executable(${LIB_NAME})
target_sources(${LIB_NAME} PRIVATE example.cpp)
target_link_libraries(${LIB_NAME} PUBLIC ${WNCK_LIBRARIES})



