project(LittleGameEngine2D)

find_package(GLFW3 CONFIG REQUIRED)
find_package(GLM CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

include(${ROOT_DIR}/cmake/include_lge2d.cmake)

file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.hpp
)

add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${LGE2D_SRC_FILES}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE glm::glm
        PRIVATE glfw
        PRIVATE Vulkan::Vulkan
)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${LGE2D_INCLUDE_DIR}
)

add_dependencies(${PROJECT_NAME} Shaders)