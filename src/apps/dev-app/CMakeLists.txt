include(../../../Cmake/Common.cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPP_HEADER_LIST "")
set(CPP_SOURCE_LIST "")

addCppSource(DevApp CPP_SOURCE_LIST source main.cpp)

buildSourceGroups(CPP_HEADER_LIST)
buildSourceGroups(CPP_SOURCE_LIST)

message("CPP_SOURCE_LIST-> ${CPP_SOURCE_LIST}")

add_executable(DevApp WIN32 ${CPP_SOURCE_LIST} ${CPP_HEADER_LIST})
target_link_libraries(DevApp PRIVATE Thor)

set_target_properties(DevApp PROPERTIES FOLDER Apps)