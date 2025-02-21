#add_library(allegro-claysrc STATIC 
    # Non-executable files go here
#)
add_executable(allegro-clay allegro-clay/Main.cpp)

target_include_directories(allegro-clay PUBLIC ./)
target_link_libraries(allegro-clay PUBLIC
    clay
    allegro
    allegro_primitives
    allegro_font
    allegro_color
    allegro_ttf
    allegro_image
)

# vim:ft=cmake
