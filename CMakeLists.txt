cmake_minimum_required(VERSION 3.10)
project(PROCON2024)

# Add an executable target
add_executable(main 
    src/planB.cpp 
    src/ops.cpp 
    src/types.cpp
)

# Specify include directories
include_directories(src/includes)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the executable
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})