cmake_minimum_required(VERSION 3.30)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(PROCON2024)
# Add an executable target
add_executable(main 
    src/Astar.cpp)

# Specify include directories
include_directories(src/includes)
include_directories(vcpkg)

target_precompile_headers(main PRIVATE src/includes/pch.h) 
target_sources(main PRIVATE src/pch.cpp)

# Link CURL
find_package(CURL REQUIRED)
target_link_libraries(main PRIVATE CURL::libcurl)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the executable
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output")

set(CMAKE_CUDA_ARCHITECTURES "native")
if (MSVC)
    set(Torch_DIR "libtorch/share/cmake/Torch")
endif (MSVC)
if (LINUX)
	set(Torch_DIR "~/libtorch/share/cmake/Torch")
endif (LINUX)
find_package(Torch REQUIRED)
target_link_libraries(main PRIVATE "${TORCH_LIBRARIES}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# Set the C++ standard
set_property(TARGET main PROPERTY CXX_STANDARD 20)

if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET main
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:main>)
endif (MSVC)
