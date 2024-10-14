cmake_minimum_required(VERSION 3.30)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(PROCON2024)
# Add an executable target
add_executable(main 
    # src/main.cpp 
    # src/ops.cpp 
    # src/types.cpp
    # src/procon_api.cpp
    src/environment.cpp
    src/weighted_astar.cpp
)   

# Specify include directories
include_directories(src/includes)
include_directories(vcpkg)

# Link CURL
#find_package(CURL REQUIRED)
#target_link_libraries(main PRIVATE CURL::libcurl)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the executable
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output")

 
