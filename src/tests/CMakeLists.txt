include(../../Cmake/Common.cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPP_HEADER_LIST "")
set(CPP_SOURCE_LIST "")

addCppSource(ThorTester CPP_SOURCE_LIST "" main.cpp)
addCppSource(ThorTester CPP_SOURCE_LIST "" tests_array_list.cpp)
addCppSource(ThorTester CPP_SOURCE_LIST "" tests_strings.cpp)
addCppSource(ThorTester CPP_SOURCE_LIST "" tests_shared_refs.cpp)

add_executable(ThorTester ${CPP_SOURCE_LIST} ${CPP_HEADER_LIST})
target_link_libraries(ThorTester PRIVATE Thor)
target_link_libraries(ThorTester PRIVATE gtest)
set_target_properties(ThorTester PROPERTIES FOLDER Tests)