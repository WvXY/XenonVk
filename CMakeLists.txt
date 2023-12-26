cmake_minimum_required(VERSION 3.26)
project(little-game-engine)

set(CMAKE_CXX_STANDARD 23)

# using vcpkg to manage dependencies
#set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
#find_path(GLM_PATH glm/glm.h)

find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)
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
)

target_link_libraries(${PROJECT_NAME}
        glfw
        glm::glm
        Vulkan::Vulkan
)

# --------------auto compile shaders---------------
## https://gist.github.com/evilactually/a0d191701cb48f157b05be7f74d79396
if (WIN32)
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
elseif (UNIX)
    set(GLSL_VALIDATOR "/bin/glslangValidator")
endif ()


file(GLOB_RECURSE GLSL_SOURCE_FILES
        "shaders/*.frag"
        "shaders/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(${PROJECT_NAME} Shaders)

# --------------copy shaders to src dir---------------
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${PROJECT_BINARY_DIR}/shaders"
#        "${CMAKE_SOURCE_DIR}/shaders"
#)
