cmake_minimum_required(VERSION 3.12)
project(Thor VERSION 1.0.0)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src/libs/thor)
add_subdirectory(src/apps/dev-app)
