add_executable(tests 
    src/ABCDTests.cpp
)

Include(FetchContent)

# TODO: Ensure this is up-to-date prior to using the code
# The template will not be kept 100% up-to-date due to this being a waste of time
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.1
)

FetchContent_MakeAvailable(Catch2)

target_link_libraries(tests Catch2::Catch2WithMain)

target_link_libraries(tests reflect-cppsrc)

# vim:ft=cmake
