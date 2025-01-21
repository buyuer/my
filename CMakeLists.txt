#
# @author  dingddding@163.com
# @date    2024.11.02
# @brief   CMake file
#

cmake_minimum_required(VERSION 3.28)

option(MYTENSOR_USE_CUDA OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(mytensor LANGUAGES CXX)

if (MYTENSOR_USE_CUDA)
    find_package(CUDAToolkit REQUIRED)
    set(CMAKE_CUDA_STANDARD 20)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    enable_language(CUDA)
endif ()

add_subdirectory(src)

add_executable(test test.cpp)
target_link_libraries(test mytensor)