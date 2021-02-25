cmake_minimum_required(VERSION 3.0.0)
project(5281568_YosuaMartiansia_A2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -lpthread)
SET(CMAKE_EXE_LINKER_FLAGS   "-L/usr/local/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(cmake/PVS-Studio.cmake)

include_directories(include)

add_subdirectory(lib)
add_subdirectory(res)
add_subdirectory(src)
