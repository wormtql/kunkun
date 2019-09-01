cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
#link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(server main.cpp json.hpp)

target_link_libraries(server ${GTK3_LIBRARIES})
#target_link_libraries(kunkun /usr/lib/x86_64-linux-gnu/libpthread.a)