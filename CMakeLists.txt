cmake_minimum_required(VERSION 3.28)

project(my LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(base)

add_executable(test test.cpp)
target_link_libraries(test myBase)