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

include(FetchContent)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()



set(LIB_NAME example)
add_executable(${LIB_NAME})
target_sources(${LIB_NAME} PRIVATE example.cpp)
target_link_libraries(${LIB_NAME} PUBLIC ${WNCK_LIBRARIES} nlohmann_json::nlohmann_json)



