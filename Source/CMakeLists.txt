project(XenonEngine)

include(${CMK_DIR}/xev-include.cmake)

file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.hpp
        ${PROJECT_SOURCE_DIR}/*.h
)

#add_executable(${PROJECT_NAME}
#        ${SOURCES}
#)
add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE glm::glm
        PRIVATE glfw
        PRIVATE Vulkan::Vulkan
        PRIVATE imgui
)


target_include_directories(${PROJECT_NAME}
        PRIVATE ${EXT_DIR}
        PRIVATE ${XEV_INCLUDE_DIR}
)

add_dependencies(${PROJECT_NAME} PreCompileShaders)