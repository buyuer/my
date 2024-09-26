add_library(myOps)

file(GLOB_RECURSE MY_BASE_MODULE_FILES *.ixx)
file(GLOB_RECURSE MY_BASE_SOURCE_FILES *.cpp *.cxx *.cc)
target_sources(myOps PUBLIC ${MY_BASE_SOURCE_FILES})
target_sources(myOps PUBLIC
        FILE_SET all_modules
        TYPE CXX_MODULES
        FILES ${MY_BASE_MODULE_FILES})