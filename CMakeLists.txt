cmake_minimum_required(VERSION 3.13)
project(kunkun)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
#link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(kunkun main.cpp LoginWindow.h LoginWindow.cpp CssLoader.cpp CssLoader.h INCLUDE.h Utils.cpp Utils.h Client.cpp Client.h MainWindow.cpp MainWindow.h ChatPanel.cpp ChatPanel.h json.hpp UserPanel.cpp UserPanel.h AddFriendPanel.cpp AddFriendPanel.h ClientUtils.cpp ClientUtils.h Thread.cpp Thread.h ConsolePanel.cpp ConsolePanel.h DataHub.cpp DataHub.h SendFileUtils.cpp SendFileUtils.h)

target_link_libraries(kunkun ${GTK3_LIBRARIES})
#target_link_libraries(kunkun /usr/lib/x86_64-linux-gnu/libpthread.a)