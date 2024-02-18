cmake_minimum_required(VERSION 3.26)
project(little-game-engine)

find_package(GLFW3 CONFIG REQUIRED) # move it to the top and run check for glfw3, same for below
find_package(GLM CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

find_path(TINY_OBJ_LOADER_PATH tiny_obj_loader.h
        HINTS ${EXTERNAL_LIB_DIR} REQUIRED
)

file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
        PRIVATE glm::glm
        PRIVATE glfw
        PRIVATE Vulkan::Vulkan
)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${TINY_OBJ_LOADER_PATH}
)

add_subdirectory(shaders)


