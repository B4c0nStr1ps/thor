include(../../../Cmake/Common.cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
message("Building Thor library")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPP_HEADER_LIST "")
set(CPP_SOURCE_LIST "")
set(OTHER_CONTENT_LIST "")

addCppHeader(Thor CPP_HEADER_LIST include/thor engine.h)
#core
addCppHeader(Thor CPP_HEADER_LIST include/thor/core memory.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core assertion.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core allocator.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core fixed_array.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core array_list.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core string.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core string_utils.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core diagnostics.h)
#core/platform
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/platform platform.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/platform windows_platform_api.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/platform windows_window.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/platform platform_string.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/platform platform_time.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/platform platform_type_traits.h)
addCppHeader(Thor CPP_HEADER_LIST include/thor/core/platform platform_application.h)

addCppSource(Thor CPP_SOURCE_LIST source engine.cpp)
#core
addCppSource(Thor CPP_SOURCE_LIST source/core allocator.cpp)
addCppSource(Thor CPP_SOURCE_LIST source/core memory.cpp)
addCppSource(Thor CPP_SOURCE_LIST source/core string_utils.cpp)
#core/platform
addCppSource(Thor CPP_SOURCE_LIST source/core/platform platform_string.cpp)
addCppSource(Thor CPP_SOURCE_LIST source/core/platform platform_time.cpp)
addCppSource(Thor CPP_SOURCE_LIST source/core/platform platform_application.cpp)
addCppSource(Thor CPP_SOURCE_LIST source/core/platform windows_window.cpp)

addFile(Thor OTHER_CONTENT_LIST "" thor.natvis)

buildSourceGroups(CPP_HEADER_LIST)
buildSourceGroups(CPP_SOURCE_LIST)

add_library(Thor STATIC ${CPP_SOURCE_LIST} ${CPP_HEADER_LIST} ${OTHER_CONTENT_LIST})
target_include_directories(Thor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(Thor PROPERTIES FOLDER Libs)