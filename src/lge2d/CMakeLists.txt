cmake_minimum_required(VERSION 3.10)
project(Little-Game-Engine-2D)

find_package(GLFW3 CONFIG REQUIRED)
find_package(GLM CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

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
        lge_model.cpp
        lge_model.hpp
        lge_game_object.hpp
        lge_renderer.hpp
        lge_renderer.cpp
        simple_render_system.cpp
        simple_render_system.hpp
)

target_link_libraries(${PROJECT_NAME}
        glm::glm
        glfw
        Vulkan::Vulkan
)

add_subdirectory(shaders)
