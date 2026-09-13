cmake_minimum_required(VERSION 3.10)
project(test-gtk)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-4.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

add_executable(test-gtk ./src/Main.cpp)
target_include_directories(
    test-gtk
PUBLIC
    ${GTKMM_INCLUDE_DIRS}
    src
)
target_link_libraries(
    test-gtk
PUBLIC
    ${GTKMM_LIBRARIES}
)
