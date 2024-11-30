cmake_minimum_required(VERSION 3.10)
project(experiment)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib
    GIT_TAG 5.5
)

FetchContent_Declare(raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui
    GIT_TAG 4.0
)

FetchContent_MakeAvailable(
    raylib
)

add_subdirectory(src)

add_custom_target(run
    COMMAND experiment
    DEPENDS experiment
    COMMENT "Run experiment")

# vim:ft=cmake
