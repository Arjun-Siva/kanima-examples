cmake_minimum_required(VERSION 3.16)
project(kanima-examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2 path (update this if needed)
set(SDL2_PATH "C:/Qt/SDL2")

# Make SDL2 available to all subdirectories
include_directories(${SDL2_PATH}/include)
link_directories(${SDL2_PATH}/lib)

# Add the kanima library
add_subdirectory(kanima)

# SDL library
add_subdirectory(sdl2_gui)

# example directories
add_subdirectory(sdl_test)
