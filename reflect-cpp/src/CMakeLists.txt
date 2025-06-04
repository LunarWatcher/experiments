#add_library(reflect-cppsrc STATIC 
    # Non-executable files go here
#)
add_executable(reflect-testing reflect-cpp/Main.cpp)
find_package(reflectcpp REQUIRED)
target_link_libraries(reflect-testing PUBLIC reflectcpp::reflectcpp)

target_include_directories(reflect-testing PUBLIC ./)

# vim:ft=cmake
