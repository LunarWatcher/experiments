set(WANT_TESTS OFF CACHE STRING "" FORCE)
set(WANT_EXAMPLES OFF CACHE STRING "" FORCE)
set(WANT_DEMO OFF CACHE STRING "" FORCE)
set(WANT_POPUP_EXAMPLES OFF CACHE STRING "" FORCE)
set(WANT_DOCS OFF CACHE STRING "" FORCE)
set(ALLEGRO_SDL OFF CACHE STRING "" FORCE)
set(WANT_NATIVE_DIALOG ON CACHE STRING "" FORCE)

set(SHARED ON CACHE STRING "" FORCE)
set(BUILD_SHARED_LIBS ON)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED_allegro5 ON)
FetchContent_Declare(allegro5
    GIT_REPOSITORY https://github.com/liballeg/allegro5
    GIT_TAG 5.2.10.0
    OVERRIDE_FIND_PACKAGE
    PATCH_COMMAND git reset --hard && git apply ${CMAKE_CURRENT_SOURCE_DIR}/3p-patches/allegro.patch
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(Freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype
    GIT_TAG VER-2-13-3
    OVERRIDE_FIND_PACKAGE
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(
    Freetype
)
set(FREETYPE_INCLUDE_DIRS ${freetype_SOURCE_DIR}/include/)
set(FREETYPE_LIBRARIES freetype)
set(FREETYPE_FOUND YES)
add_definitions(-DFT_FREETYPE_H=<freetype/freetype.h>)

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
    ${allegro5_SOURCE_DIR}/addons/color/
    ${allegro5_SOURCE_DIR}/addons/main/
    ${allegro5_SOURCE_DIR}/addons/primitives/
)
# Global include required for shit to cooperate. The demos run standalone anyway, so this won't pollute anything
include_directories(PUBLIC
    ${ALLEGRO_INCLUDE_DIRS}
)
