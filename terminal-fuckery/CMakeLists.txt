cmake_minimum_required(VERSION 3.10)
project(terminal-fuckery)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(stc
    GIT_REPOSITORY https://github.com/LunarWatcher/stc
    # Optional: pin stc version
    # GIT_TAG <hash>
)
FetchContent_MakeAvailable(stc)

add_subdirectory(src)
#add_subdirectory(tests)

add_custom_target(run
    COMMAND terminal-fuckery
    DEPENDS terminal-fuckery
    COMMENT "Run terminal-fuckery")
#add_custom_target(test
    #COMMAND tests
    #DEPENDS tests
    #COMMENT "Test terminal-fuckery")
# vim:ft=cmake
