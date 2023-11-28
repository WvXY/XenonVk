cmake_minimum_required(VERSION 3.26)
project(simple_game_engine)

set(CMAKE_CXX_STANDARD 23)

# using vcpkg to manage dependencies
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

find_package(GLM CONFIG REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
)

target_link_libraries(${PROJECT_NAME}
        glfw
        glm::glm
        Vulkan::Vulkan
)

