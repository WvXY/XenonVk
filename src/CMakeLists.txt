project(XenonApp)

include(${CMK_DIR}/include_xev.cmake)

file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.hpp
)

add_executable(${PROJECT_NAME}
        ${SOURCES}
        #        ${VKE_SRC_FILES}
)


target_link_libraries(${PROJECT_NAME}
        PRIVATE glm::glm
        PRIVATE glfw
        PRIVATE Vulkan::Vulkan
)


target_include_directories(${PROJECT_NAME}
        PRIVATE ${EXT_DIR}
        PRIVATE ${XEV_INCLUDE_DIR}
)

add_dependencies(${PROJECT_NAME} CompileShaders)