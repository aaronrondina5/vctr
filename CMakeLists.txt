# vctr

cmake_minimum_required(VERSION 3.12)

project(vctr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-stdlib=libc++)
endif()

add_subdirectory(src)
add_subdirectory(tests)