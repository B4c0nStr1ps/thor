cmake_minimum_required(VERSION 3.12)
project(Thor VERSION 1.0.0)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#build external requirments

#gtests
# Download and unpack googletest at configure time
configure_file(cmake/CMakeLists.txt.gtests.it googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

set_target_properties(gtest_main PROPERTIES FOLDER External)
set_target_properties(gtest PROPERTIES FOLDER External)
#end gtests

add_subdirectory(src/libs/thor)
add_subdirectory(src/apps/dev-app)
add_subdirectory(src/tests)
