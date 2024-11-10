cmake_minimum_required(VERSION 3.12)
project(example_project)
include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FETCHCONTENT_UPDATES_DISCONNECTED_glfw ON)
set(FETCHCONTENT_FULLY_DISCONNECTED_glfw OFF)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
  # we set source dir to current ./libs/ so code analysis would work as normal
  # since we include headers of it
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw
)
FetchContent_MakeAvailable(glfw)

get_property(INCLUDE_PATHS GLOBAL PROPERTY "FRE2D_INCLUDE_PATHS")
get_property(SOURCE_FILES GLOBAL PROPERTY "FRE2D_SOURCE_FILES")

set(INCLUDE_PATHS ${INCLUDE_PATHS} ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include/)

add_executable(example_project example.cpp)
target_include_directories(example_project PRIVATE ${INCLUDE_PATHS})
target_link_libraries(example_project PRIVATE glfw fre2d_lib)