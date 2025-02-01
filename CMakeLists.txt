cmake_minimum_required(VERSION 3.28)
project(CustomLogger)

set(CMAKE_CXX_STANDARD 17)

add_executable(CustomLogger
        cmake-build-debug/src/logger.cpp
        cmake-build-debug/src/main.cpp
        cmake-build-debug/src/logger.hpp)
