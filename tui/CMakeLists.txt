cmake_minimum_required(VERSION 3.10)
project(tui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test-tui ./src/Main.cpp)
target_include_directories(
    test-tui
PUBLIC
    src
)
