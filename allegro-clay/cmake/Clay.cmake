include(FetchContent)

set(CLAY_INCLUDE_ALL_EXAMPLES OFF CACHE STRING "" FORCE)

FetchContent_Declare(clay 
    GIT_REPOSITORY https://github.com/nicbarker/clay/
    GIT_TAG v0.13
)
FetchContent_MakeAvailable(clay)

add_library(clay INTERFACE)
target_include_directories(clay INTERFACE ${clay_SOURCE_DIR})
target_sources(clay INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/meta/Clay.cpp)
