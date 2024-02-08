cmake_minimum_required(VERSION 3.26)
project(little-game-engine)

find_package(GLFW3 CONFIG REQUIRED)
find_package(GLM CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

find_path(TINY_OBJ_LOADER_PATH tiny_obj_loader.h
        HINTS ${EXTERNAL_LIB_DIR} REQUIRED
)

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
        lge_camera.cpp
        lge_camera.hpp
        kbd_controller.cpp
        kbd_controller.hpp
        lge_utils.hpp
        lge_game_object.cpp
        lge_buffer.cpp
        lge_buffer.hpp
        lge_frame_info.hpp
        lge_descriptors.cpp
        lge_descriptors.hpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE glm::glm
        PRIVATE glfw
        PRIVATE Vulkan::Vulkan
)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${TINY_OBJ_LOADER_PATH}
)

add_subdirectory(shaders)


