cmake_minimum_required(VERSION 3.20)
project(ZAdanie_3 C)

set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")

add_executable(ZAdanie_3 main.c
        z3.c)
