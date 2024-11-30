#add_library(factorio-stacksrc STATIC 
    # Non-executable files go here
#)
add_executable(factorio-stack factorio-stack/Main.cpp)



target_include_directories(
    factorio-stack
PUBLIC
    ./
)

target_link_libraries(
    factorio-stack 
PUBLIC 
    agui_allegro5

    ${ALLEGRO_TARGETS}
)

# vim:ft=cmake
