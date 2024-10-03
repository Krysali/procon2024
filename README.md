# XStar - procon2024
Development environment is Visual Studio Code with g++ compiler, CMake build tools with Ninja. 
## Prerequisites
- **git**: https://git-scm.com/downloads/
- **CMake**: https://cmake.org/download/
- **MSYS2**: https://www.msys2.org/
- **C++ Compiler**:
  - Add `C:\msys64\ucrt64\bin` and `C:\msys64\mingw64\bin` into PATH (Edit the system environment variables)
  - In MSYS2 UCRT64 console, `pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain`
  - When it finishes, `pacman -S mingw-w64-x86_64-ninja`

## Setup
### Initialize repository
in some folder, type `https://github.com/LEveLiQ/procon2024.git`
### Download vcpkg & libcurl
first, run 
`git clone https://github.com/microsoft/vcpkg.git`\
then `cd vcpkg && bootstrap-vcpkg.bat`

in `procon2024` folder, type:\
`vcpkg integrate install`\
`vcpkg install curl`

inside `CMakeLists.txt`, make sure these lines exist:
```CMake
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
```
```CMake
find_package(CURL REQUIRED)
target_link_libraries(main PRIVATE CURL::libcurl)
```
if there is # before them, **REMOVE** the #, otherwise `procon_api` will not work.

### Initialize CMake
first install the **C/C++ Extension Pack** in the Extensions page.

`Ctrl + S` the `CMakeLists.txt` file, CMake Tools menu will come up. select the Debug button, and the program will build itself\
to add a new file, simply modify the following section:
```CMake
add_executable(main # add your filenames relative to project root (procon2024)
    src/main.cpp 
    src/ops.cpp 
    src/types.cpp
    src/procon_api.cpp
)
```
you don't have to add headers seperately, just put them in `src\includes`
