cmake_minimum_required(VERSION 3.28)

include(FetchContent)
FetchContent_Declare(conan
    GIT_REPOSITORY https://github.com/conan-io/cmake-conan
    GIT_TAG develop2
)
FetchContent_MakeAvailable(conan)
set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES ${conan_SOURCE_DIR}/conan_provider.cmake)

project(reflect-cpp-testing)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
#add_subdirectory(tests)

add_custom_target(run
    COMMAND reflect-testing
    DEPENDS reflect-testing
    COMMENT "Run reflect-testing")

# vim:ft=cmake
