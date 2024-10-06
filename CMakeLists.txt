cmake_minimum_required(VERSION 3.30)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(PROCON2024)
# Add an executable target
add_executable(main 
<<<<<<< HEAD
    src/Goku.cpp 
    src/types.cpp
=======
    src/main.cpp
    src/types.cpp
    src/json_ops.cpp
    src/procon_api.cpp
>>>>>>> 0b599747e9fded3e4e426f81e3508a9d96d85d14
)

# Specify include directories
include_directories(src/includes)
include_directories(vcpkg)

# Link CURL
find_package(CURL REQUIRED)
target_link_libraries(main PRIVATE CURL::libcurl)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the executable
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output")