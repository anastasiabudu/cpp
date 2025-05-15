cmake_minimum_required(VERSION 3.20.0)
project(z2 C)

set(CMAKE_C_STANDARD 11)

add_executable(z2 main.c
        z2.c
        data.h
        functions.h
        data.c
        functions.c)
target_link_libraries(z2 PRIVATE m)