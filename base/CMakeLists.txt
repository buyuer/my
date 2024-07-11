add_library(myBase)

file(GLOB_RECURSE MY_BASE_MODULE_FILES *.ixx)
file(GLOB_RECURSE MY_BASE_SOURCE_FILES *.cpp *.cxx *.cc)
target_sources(myBase PUBLIC ${MY_BASE_SOURCE_FILES})
target_sources(myBase PUBLIC
        FILE_SET all_modules
        TYPE CXX_MODULES
        FILES ${MY_BASE_MODULE_FILES})