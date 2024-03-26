project(LittleGameEngine2D)

include(${CMK_DIR}/include_lge2d.cmake)

file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.hpp
)

add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${LGE2D_SRC_FILES}
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
        PRIVATE ${LGE2D_INCLUDE_DIR}
)

add_dependencies(${PROJECT_NAME} CompileShaders)