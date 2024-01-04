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

set(TEST_SOURCES
  vctr.t.cpp
)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    gtest_main
    gtest
    
    vctr
)

target_include_directories(${PROJECT_NAME} PUBLIC ../include/)