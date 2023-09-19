cmake_minimum_required(VERSION 3.27)

project(ISP_RAS)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
