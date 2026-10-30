cmake_minimum_required(VERSION 4.3)
project(dvoinoi_massiv C)

set(CMAKE_C_STANDARD 11)

add_executable(dvoinoi_massiv
        dynamic_massiv.c
        dynamic_massiv.h
        main.c)
