cmake_minimum_required(VERSION 3.20)
project(z5 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -lm")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")

add_executable(z5 z5.c
        monopoly.c
        monopoly.h
        z5.c)
