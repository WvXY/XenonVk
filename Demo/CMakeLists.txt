project(XenonApp)

include(${CMK_DIR}/xev-include.cmake)
#add_subdirectory()


file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.hpp
        ${PROJECT_SOURCE_DIR}/*.h
)
message(STATUS "XenonApp: ${PROJECT_SOURCE_DIR}")

add_executable(${PROJECT_NAME}
        ${SOURCES}
)

target_sources(
        ${PROJECT_NAME}
        PRIVATE ${XEV_INCLUDE_FILES}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE glm::glm
        PRIVATE glfw
        PRIVATE Vulkan::Vulkan
        PRIVATE XenonEngine
)


target_include_directories(${PROJECT_NAME}
        PRIVATE ${EXT_DIR}
        PRIVATE ${XEV_INCLUDE_DIR}
)

