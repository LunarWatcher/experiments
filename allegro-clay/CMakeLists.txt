cmake_minimum_required(VERSION 3.10)
project(allegro-clay)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/CompileFlags.cmake)
include(cmake/Allegro.cmake)
include(cmake/Clay.cmake)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(src)
#add_subdirectory(tests)

add_custom_target(run
    COMMAND allegro-clay
    DEPENDS allegro-clay
    COMMENT "Run allegro-clay")
#add_custom_target(test
    #COMMAND tests
    #DEPENDS tests
    #COMMENT "Test allegro-clay")
# vim:ft=cmake
