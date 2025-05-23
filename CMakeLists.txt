cmake_minimum_required(VERSION 3.26)
project(XenonVk)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------   Paths   -------------------
set(AST_DIR ${CMAKE_SOURCE_DIR}/Assets)
set(CMK_DIR ${CMAKE_SOURCE_DIR}/CMake)
set(EXT_DIR ${CMAKE_SOURCE_DIR}/Engine/External)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/Engine)
set(SHA_DIR ${CMAKE_SOURCE_DIR}/Engine/Renderer/Shaders)
set(SPIRV_OUT_DIR ${CMAKE_BINARY_DIR}/spirv)

# ------------------- Options -------------------
## GLFWCMakeTemplate https://github.com/SasLuca/GLFWCMakeTemplate
#if (UNIX)   # use wayland on linux
#    option(GLFW_USE_WAYLAND "Enable Wayland" ON)
#endif ()

# -------------- Dependencies -------------------
include(CMake/autoconfig-dependencies.cmake)
include(CMake/precompile-shaders.cmake)

# ------------------- Project -------------------
add_subdirectory(Engine)
add_subdirectory(Example)
