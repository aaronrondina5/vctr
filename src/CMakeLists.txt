# vctr/src

add_library(vctr
    vector.cpp
)

target_include_directories(vctr PUBLIC ${CMAKE_SOURCE_DIR}/include)