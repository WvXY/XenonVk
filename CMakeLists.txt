cmake_minimum_required(VERSION 3.26)
project(LittleGameEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX)
    set(GLFW_USE_WAYLAND ON)    # use wayland on linux
endif ()

# set global root directory
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ASSETS_DIR ${ROOT_DIR}/assets)

# set vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_FILE_DIR ${ROOT_DIR}/cmake)
set(EXTERNAL_LIB_DIR ${ROOT_DIR}/external/lib)
set(CUSTOM_INCLUDE_DIR ${ROOT_DIR}/include)

# set output directory
set(OUT_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME})    # TODO: change this
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})

# set shader output directory
set(SHADER_SPIRV_OUTPUT_DIRECTORY ${OUT_DIR}/shaders)

# script to compile shaders
include(${ROOT_DIR}/cmake/compile_shaders.cmake)

add_subdirectory(src)
add_subdirectory(examples/lge2d)
