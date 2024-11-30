add_library(experimentsrc STATIC 
    experiment/Window.cpp
)
add_executable(experiment experiment/Main.cpp)

target_include_directories(experimentsrc PUBLIC ./)
target_link_libraries(
    experimentsrc 
PUBLIC 
    raylib
)

target_link_libraries(experiment experimentsrc)

# vim:ft=cmake
