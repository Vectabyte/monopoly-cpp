cmake_minimum_required(VERSION 3.10)

project(monopoly LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force UTF-8 source and execution charset on MSVC
if (MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(NOMINMAX)
endif()

add_executable(monopoly monopoly.cpp)
