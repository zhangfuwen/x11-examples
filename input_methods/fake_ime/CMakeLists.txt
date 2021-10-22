project(fake_ime)

cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)

find_package(PkgConfig)
pkg_check_modules(IBus REQUIRED ibus-1.0)
if(IBus_FOUND)
    include_directories(${IBus_INCLUDE_DIRS})
    message("IBus_INCLUDE_DIRS ${IBus_INCLUDE_DIRS}")
    link_directories(${IBus_LIBRARY_DIRS})
endif(IBus_FOUND)

pkg_check_modules(GLib2 REQUIRED glib-2.0)

if(GLib2_FOUND)
    include_directories(${GLib2_INCLUDE_DIRS})
    link_directories(${GLib2_LIBRARY_DIRS})
    message("GLib2_INCLUDE_DIRS ${GLib2_INCLUDE_DIRS}")
else()
    message("glib2 not found")
endif(GLib2_FOUND)

add_executable(fake_ime fakeime_main.cpp)
target_link_libraries(fake_ime ${IBus_LIBRARIES} ${GLib2_LIBRARIES} dl pthread)
