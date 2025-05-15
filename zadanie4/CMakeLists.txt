cmake_minimum_required(VERSION 3.20)
project(ZADANIE4 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")

add_executable(ZADANIE4 main.c
        monopoly.h
        monopoly.c
        z4.c)
