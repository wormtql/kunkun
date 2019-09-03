cmake_minimum_required(VERSION 3.13)
project(server)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
#link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

execute_process(COMMAND mysql_config --cflags
        OUTPUT_VARIABLE MYSQL_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND mysql_config --libs
        OUTPUT_VARIABLE MYSQL_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

add_executable(server main.cpp Database.h Database.cpp)

SET(CMAKE_C_FLAGS"${CMAKE_C_FLAGS}${GCC_MYSQL_COMPILE_FLAGS}")
target_link_libraries(server ${GTK3_LIBRARIES})
target_link_libraries(server ${MYSQL_LIBS})

