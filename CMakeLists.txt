cmake_minimum_required(VERSION 3.26)
project(LittleGameEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if (UNIX)
#    set(GLFW_USE_WAYLAND ON)    # use wayland on linux
#endif ()

if (NOT UNIX)
    find_package(GLFW3 REQUIRED)
    find_package(GLM REQUIRED)
#    find_package(Vulkan REQUIRED)
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

add_subdirectory(examples/demo2)
add_subdirectory(examples/demo3)
