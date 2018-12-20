include(../../Cmake/Common.cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPP_HEADER_LIST "")
set(CPP_SOURCE_LIST "")

addCppSource(ThorTester CPP_SOURCE_LIST "" tests_array_list.cpp)

add_executable(ThorTester ${CPP_SOURCE_LIST} ${CPP_HEADER_LIST})
target_link_libraries(ThorTester PRIVATE Thor)
target_link_libraries(ThorTester PRIVATE gtest_main)
set_target_properties(ThorTester PROPERTIES FOLDER Tests)