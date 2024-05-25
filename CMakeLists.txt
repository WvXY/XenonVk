cmake_minimum_required(VERSION 3.26)
project(XenonVk)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX)
    option(GLFW_USE_WAYLAND "Enable Wayland" ON)    # use wayland on linux
endif ()

if (NOT UNIX)
    find_package(GLFW3 REQUIRED)
    find_package(GLM REQUIRED)
endif ()

find_package(Vulkan REQUIRED)


# set vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

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
