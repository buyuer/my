#
# @author  dingddding@163.com
# @date    2024.11.02
# @brief   CMake file
#

cmake_minimum_required(VERSION 3.28)

project(mytensor LANGUAGES CXX CUDA)

option(MYTENSOR_USE_CUDA OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)

add_executable(test test.cpp)
target_link_libraries(test mytensor)