cmake_minimum_required(VERSION 3.10)
project(test-qt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup(REQUIRES 6.5)

include(FetchContent)
FetchContent_declare(stc
    GIT_REPOSITORY https://github.com/LunarWatcher/stc
    GIT_TAG v2026-09-13
)
FetchContent_MakeAvailable(stc)

qt_add_executable(test-qt ./src/Main.cpp)
target_include_directories(
    test-qt
PUBLIC
    src
)

target_link_libraries(test-qt PUBLIC Qt6::Core Qt6::Widgets stc)
