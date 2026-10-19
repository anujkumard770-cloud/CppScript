
cmake_minimum_required(VERSION 3.15)

project(CppScript
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cppscript
    src/main.cpp
    src/lexer.cpp
)

target_include_directories(cppscript
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
