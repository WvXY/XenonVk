cmake_minimum_required(VERSION 3.26)
project(little-game-engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLFW_USE_WAYLAND ON)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(OUT_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})

# if(MSVC)  # incase of Clion & MSVC, we may need shader at 2 places
# set(SHADER_SPIRV_OUTPUT_DIRECTORY ${OUT_DIR}/$<CONFIG>/shaders)
# else()
# set(SHADER_SPIRV_OUTPUT_DIRECTORY ${OUT_DIR}/shaders)
# endif()
set(SHADER_SPIRV_OUTPUT_DIRECTORY ${OUT_DIR}/shaders)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

add_subdirectory(src)
add_subdirectory(src/lge2d)
