cmake_minimum_required(VERSION 3.26)
project(little-game-engine)

set(CMAKE_CXX_STANDARD 23)

# using vcpkg to manage dependencies
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

find_package(GLM CONFIG REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(Vulkan REQUIRED)

# TODO: auto compile shaders

add_executable(${PROJECT_NAME}
        main.cpp
        first_app.hpp
        first_app.cpp
        lge_window.hpp
        lge_window.cpp
        lge_pipeline.hpp
        lge_pipeline.cpp
        lge_device.hpp
        lge_device.cpp
        lge_swap_chain.hpp
        lge_swap_chain.cpp
)

target_link_libraries(${PROJECT_NAME}
#        shaders
        glfw
        glm::glm
        Vulkan::Vulkan
)

