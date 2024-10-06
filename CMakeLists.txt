cmake_minimum_required(VERSION 3.12)
project(fre2d_lib)
include(FetchContent)
option(BUILD_EXAMPLE "Build example to see if fre2d works correctly" ON)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
  # we set source dir to current ./libs/ so code analysis would work as normal
  # since we include headers of it
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw
)
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
  GIT_TAG VER-2-13-3
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype
)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
)
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb
)
FetchContent_Declare(
  colorized
  GIT_REPOSITORY https://github.com/ferhatgec/colorized.git
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/colorized
)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(freetype)
FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(colorized)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INCLUDE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb/
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm)

# macOS is an exception here, but idc for now.
find_package(OpenGL 4.5 REQUIRED)

file(GLOB_RECURSE SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_library(fre2d_lib ${SOURCE_FILES}
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include/glad/glad.h
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/glad.c)
target_include_directories(fre2d_lib PRIVATE ${INCLUDE_PATHS})
target_link_libraries(fre2d_lib PRIVATE glfw freetype OpenGL::GL)

if(BUILD_EXAMPLE)
  set_property(GLOBAL PROPERTY FRE2D_INCLUDE_PATHS ${INCLUDE_PATHS})
  set_property(GLOBAL PROPERTY FRE2D_SOURCE_FILES ${SOURCE_FILES})
  add_subdirectory("example")
endif()