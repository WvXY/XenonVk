project(LittleGameEngine3D)

include(${CMK_DIR}/include_lge3d.cmake)
#include(${CMK_DIR}/include_core.cmake)

file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.hpp
)

add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${LGE3D_SRC_FILES}
)

if (NOT UNIX)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE glm::glm
            PRIVATE glfw
            PRIVATE Vulkan::Vulkan
    )
else ()
    target_link_libraries(${PROJECT_NAME}
            glfw
            PRIVATE Vulkan::Vulkan
    )
endif ()

target_include_directories(${PROJECT_NAME}
        PRIVATE ${EXT_DIR}
        PRIVATE ${LGE3D_INCLUDE_DIR}
)

add_dependencies(${PROJECT_NAME} CompileShaders)