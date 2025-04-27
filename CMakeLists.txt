cmake_minimum_required(VERSION 3.10)

project(interpreter CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_COMPILER g++)

add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)

set(SOURCES 
    src/main.cpp
    src/token/token.hpp
    src/token/token.cpp
    src/lexer/lexer.hpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/parser/parser.hpp
    src/ast/ast.cpp
    src/ast/ast.hpp
    src/ast/astPrinter.hpp
    src/ast/astPrinter.cpp
    src/interpreter/interpreter.hpp
    src/interpreter/interpreter.cpp
    src/value.hpp
    src/value.cpp
)

add_executable(interpreter ${SOURCES})