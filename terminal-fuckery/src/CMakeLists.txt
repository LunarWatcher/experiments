add_executable(
    terminal-fuckery terminal-fuckery/Main.cpp
)
add_executable(
    mocksub terminal-fuckery/MockSub.cpp
)
target_link_libraries(terminal-fuckery PUBLIC stc)
target_link_libraries(mocksub PUBLIC stc)

target_include_directories(terminal-fuckery PUBLIC ./)
target_include_directories(mocksub PUBLIC ./)

# vim:ft=cmake
