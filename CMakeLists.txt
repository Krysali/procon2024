cmake_minimum_required(VERSION 3.10)
project(PROCON2024)

# Add an executable target
add_executable(prog 
    src/main.cpp 
    src/ops.cpp 
    src/types.cpp
)

# Specify include directories
include_directories(src/includes)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)