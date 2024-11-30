cmake_minimum_required(VERSION 3.10)
project(factorio-stack)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Allegro config vars
set(WANT_TESTS OFF CACHE STRING "" FORCE)
set(WANT_EXAMPLES OFF CACHE STRING "" FORCE)
set(WANT_DEMO OFF CACHE STRING "" FORCE)
set(WANT_POPUP_EXAMPLES OFF CACHE STRING "" FORCE)
set(WANT_DOCS OFF CACHE STRING "" FORCE)
set(ALLEGRO_SDL ON CACHE STRING "" FORCE)

set(WANT_SHARED ON CACHE STRING "" FORCE)


include(FetchContent)

FetchContent_Declare(allegro5
    GIT_REPOSITORY https://github.com/liballeg/allegro5
    GIT_TAG 5.2.10.0
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-2.30.9
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(Agui
    GIT_REPOSITORY https://github.com/jmasterx/Agui
    GIT_TAG d962206bb4b97e6a9f7bd6b2050fbbef493ea66f
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(
    allegro5
)

# For some reason, Allegro's targets doesn't include header include dirs
# This probably isn't enough to cover everything either, it's apparently supposed to be covered by 
# `make install`.
set (ALLEGRO_INCLUDE_DIRS
    ${allegro5_SOURCE_DIR}/include/
    ${allegro5_BINARY_DIR}/include/
    ${allegro5_SOURCE_DIR}/addons/image/
    ${allegro5_SOURCE_DIR}/addons/ttf/
    ${allegro5_SOURCE_DIR}/addons/font/
    ${allegro5_SOURCE_DIR}/addons/video/
    ${allegro5_SOURCE_DIR}/addons/audio/
    ${allegro5_SOURCE_DIR}/addons/primitives/
)

# Global include required for agui to cooperate
include_directories(PUBLIC
    ${ALLEGRO_INCLUDE_DIRS}
)

FetchContent_MakeAvailable(
    Agui
)

add_subdirectory(src)
#add_subdirectory(tests)

add_custom_target(run
    COMMAND factorio-stack
    DEPENDS factorio-stack
    COMMENT "Run factorio-stack")
# vim:ft=cmake
