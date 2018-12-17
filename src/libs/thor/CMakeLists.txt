include(../../../Cmake/Common.cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
message("Building Thor library")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPP_HEADER_LIST "")
set(CPP_SOURCE_LIST "")

addCppHeader(Thor CPP_HEADER_LIST include/thor thor.h)
addCppSource(Thor CPP_SOURCE_LIST source thor.cpp)

buildSourceGroups(CPP_HEADER_LIST)
buildSourceGroups(CPP_SOURCE_LIST)

add_library(Thor STATIC ${CPP_SOURCE_LIST} ${CPP_HEADER_LIST})
target_include_directories(Thor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(Thor PROPERTIES FOLDER Libs)