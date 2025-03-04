cmake_minimum_required(VERSION 3.10)

project(interpreter C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_COMPILER gcc)

add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)

set(SOURCES 
    src/main.c
)   

add_executable(interpreter ${SOURCES})