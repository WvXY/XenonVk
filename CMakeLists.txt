cmake_minimum_required(VERSION 3.26)
project(XenonVk)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------- Dependencies -------------------
# Vulkan SDK
find_package(Vulkan REQUIRED)

include(FetchContent)

# GLFWCMakeTemplate https://github.com/SasLuca/GLFWCMakeTemplate
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

#if (UNIX)
#    option(GLFW_USE_WAYLAND "Enable Wayland" ON)    # use wayland on linux
#endif ()

# Math lib: GLM
FetchContent_Declare(glm GIT_REPOSITORY	https://github.com/g-truc/glm.git GIT_TAG master)
FetchContent_MakeAvailable(glm)

# ------------------- Project -------------------
# set directory paths
set(AST_DIR ${CMAKE_SOURCE_DIR}/assets)
set(CMK_DIR ${CMAKE_SOURCE_DIR}/cmake)
set(EXT_DIR ${CMAKE_SOURCE_DIR}/external)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(SHA_DIR ${CMAKE_SOURCE_DIR}/shaders)

# set shader output directory
set(SPIRV_OUT_DIR ${PROJECT_BINARY_DIR}/shaders)

# script to compile shaders
include(cmake/compile_shaders.cmake)

add_subdirectory(src)
