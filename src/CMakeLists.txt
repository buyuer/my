#
# @author  dingddding@163.com
# @date    2024.11.02
# @brief   CMake file
#

add_library(mytensor)

file(GLOB_RECURSE MY_BASE_MODULE_FILES *.ixx)
file(GLOB_RECURSE MY_BASE_SOURCE_FILES *.cpp *.cxx *.cc)
target_sources(mytensor PUBLIC ${MY_BASE_SOURCE_FILES})
target_sources(mytensor PUBLIC
        FILE_SET all_modules
        TYPE CXX_MODULES
        FILES ${MY_BASE_MODULE_FILES})