cmake_minimum_required(VERSION 3.10)

project(interpreter C)

set(CMAKE_C_STANDARD 11)

set(SOURCES 
    src/main.c
)   

add_executable(interpreter ${SOURCES})