include(../../../Cmake/Common.cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
message("Building Thor library")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPP_HEADER_LIST "")
set(CPP_SOURCE_LIST "")
set(OTHER_CONTENT_LIST "")

addCppHeader(Thor CPP_HEADER_LIST include/thor thor.h)
#core
addCppHeader(Thor CPP_HEADER_LIST include/thor/core core_types.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core memory.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core assertion.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core allocator.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core fixed_array.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core array_list.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core string.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core string_utils.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core thor_traits.h)
#core/hal
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/hal platform.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/hal windows_platform_api.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/hal platform_string.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/hal platform_time.h)

addCppSource(Thor CPP_SOURCE_LIST source thor.cpp)
#core
addCppSource(Thor CPP_SOURCE_LIST source/core allocator.cpp)
addCppSource(Thor CPP_SOURCE_LIST source/core memory.cpp)
addCppSource(Thor CPP_SOURCE_LIST source/core string_utils.cpp)
#core/hal
addCppSource(Thor CPP_SOURCE_LIST source/core/hal platform_string.cpp)
addCppSource(Thor CPP_SOURCE_LIST source/core/hal platform_time.cpp)

addFile(Thor OTHER_CONTENT_LIST "" thor.natvis)

buildSourceGroups(CPP_HEADER_LIST)
buildSourceGroups(CPP_SOURCE_LIST)

add_library(Thor STATIC ${CPP_SOURCE_LIST} ${CPP_HEADER_LIST} ${OTHER_CONTENT_LIST})
target_include_directories(Thor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(Thor PROPERTIES FOLDER Libs)