project(LittleGameEngine3D)

find_package(GLFW3 CONFIG REQUIRED) # move it to the top and run check for glfw3, same for below
find_package(GLM CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

include(${ROOT_DIR}/cmake/include_lge3d.cmake)
include(${ROOT_DIR}/cmake/include_core.cmake)

file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.hpp
)

add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${LGE3D_SRC_FILES}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE glm::glm
        PRIVATE glfw
        PRIVATE Vulkan::Vulkan
)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${EXTERNAL_LIB_DIR}
        PRIVATE ${LGE3D_INCLUDE_DIR}
        PRIVATE ${LGECore_INCLUDE_DIR}
)

add_dependencies(${PROJECT_NAME} Shaders)