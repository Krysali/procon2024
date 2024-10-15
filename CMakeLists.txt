cmake_minimum_required(VERSION 3.30)

project(PROCON2024)
# Add an executable target
add_executable(main 
    # src/main.cpp 
    # src/ops.cpp 
    # src/types.cpp
    # src/procon_api.cpp
    "Train/cpp/model.cpp"
)

# Specify include directories
include_directories("src/includes")

# Link CURL
find_package(CURL REQUIRED)
target_link_libraries(main PRIVATE CURL::libcurl)

set(CMAKE_CUDA_ARCHITECTURES "native")
set(Torch_DIR "~/libtorch/share/cmake/Torch")
find_package(Torch REQUIRED)
target_link_libraries(main PRIVATE "${TORCH_LIBRARIES}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# Set the C++ standard
set_property(TARGET main PROPERTY CXX_STANDARD 20)
# Set the output directory for the executable
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "output")

if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET main
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:main>)
endif (MSVC)