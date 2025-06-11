#add_library(drogon-testsrc STATIC 
    # Non-executable files go here
#)

find_package(Drogon REQUIRED)

add_executable(drogon-test drogon-test/Main.cpp)

target_include_directories(drogon-test PUBLIC ./)
target_link_libraries(drogon-test Drogon::Drogon)

# vim:ft=cmake
