cmake_minimum_required(VERSION 3.28)
project(tests C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
find_package(cmocka REQUIRED)

add_executable(tests main.c
        ../queue.c
        ../queue.h
)

target_include_directories(
        tests
        PRIVATE
        /opt/homebrew/Cellar/cmocka/1.1.7/include
)
target_link_libraries(
        tests
        PRIVATE
        /opt/homebrew/Cellar/cmocka/1.1.7/lib/libcmocka.dylib
)