cmake_minimum_required(VERSION 3.24)
project(cw_tests C)

set(CMAKE_C_STANDARD 99)

add_executable(cw_tests main.h main.c exceptions.h image.h image.c file.h file.c memory.c memory.h exceptions.c)
