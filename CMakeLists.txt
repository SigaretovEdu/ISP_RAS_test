cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(ISP_RAS)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp karnaugh.cpp)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
