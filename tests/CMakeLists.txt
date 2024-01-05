# vctr/tests

cmake_minimum_required(VERSION 3.12)

project(vctrtests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(vctrtests

  vector.t.cpp

)

include(GoogleTest)
gtest_discover_tests(vctrtests)

target_link_libraries(vctrtests
  GTest::gtest_main

  vctr
)

target_include_directories(vctrtests PUBLIC ../include/)