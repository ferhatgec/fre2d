cmake_minimum_required(VERSION 3.12)
project(example_project)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_property(INCLUDE_PATHS GLOBAL PROPERTY "FRE2D_INCLUDE_PATHS")
get_property(SOURCE_FILES GLOBAL PROPERTY "FRE2D_SOURCE_FILES")

add_executable(example_project example.cpp)
target_include_directories(example_project PRIVATE ${INCLUDE_PATHS})
target_link_libraries(example_project PRIVATE fre2d_lib)