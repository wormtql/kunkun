cmake_minimum_required(VERSION 3.13)
project(homework)

set(CMAKE_CXX_STANDARD 14)

#include_directories(
#        /usr/include/gtk-3.0
#        /usr/include/at-spi2-atk/2.0
#        /usr/include/at-spi-2.0
#        /usr/include/dbus-1.0
#        /usr/lib/x86_64-linux-gnu/dbus-1.0/include
#        /usr/include/gtk-3.0
#        /usr/include/gio-unix-2.0/
#        /usr/include/cairo
#        /usr/include/pango-1.0
#        /usr/include/harfbuzz
#        /usr/include/pango-1.0
#        /usr/include/atk-1.0
#        /usr/include/cairo
#        /usr/include/pixman-1
#        /usr/include/freetype2
#        /usr/include/libpng16
#        /usr/include/freetype2
#        /usr/include/libpng16
#        /usr/include/gdk-pixbuf-2.0
#        /usr/include/libpng16
#        /usr/include/glib-2.0
#        /usr/lib/x86_64-linux-gnu/glib-2.0/include
#)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(homework main.cpp LoginWindow.h LoginWindow.cpp CssLoader.cpp CssLoader.h INCLUDE.h Utils.cpp Utils.h NetworkUtils.cpp NetworkUtils.h MainWindow.cpp MainWindow.h)

target_link_libraries(homework ${GTK3_LIBRARIES})