cmake_minimum_required(VERSION 3.8)
project(2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp CommandAndOptions.cpp CommandAndOptions.h)

add_executable(2 ${SOURCE_FILES})
