#
# @author  dingddding@163.com
# @date    2024.11.02
# @brief   CMake file
#

add_library(mytensor)

if (MYTENSOR_USE_CUDA)
    target_compile_definitions(mytensor PRIVATE MYTENSOR_USE_CUDA)
    file(GLOB_RECURSE MYTENSOR_CUDA_FILES *.cu)
    target_link_libraries(mytensor PUBLIC CUDA::cudart)
endif ()

file(GLOB_RECURSE MYTENSOR_MODULE_FILES *.ixx)
file(GLOB_RECURSE MYTENSOR_SOURCE_FILES *.cpp *.cxx *.cc)
target_sources(mytensor PUBLIC ${MYTENSOR_SOURCE_FILES} ${MYTENSOR_CUDA_FILES})
target_sources(mytensor PUBLIC
        FILE_SET all_modules
        TYPE CXX_MODULES
        FILES ${MYTENSOR_MODULE_FILES})
