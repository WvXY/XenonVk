# compile shaders

if(WIN32)
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
elseif(UNIX)
    set(GLSL_VALIDATOR "/bin/glslangValidator")
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
        "src/shaders/*.frag"
        "src/shaders/*.vert"
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

#add_dependencies(${PROJECT_NAME} Shaders)

# --------------copy shaders to src dir---------------
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
# COMMAND ${CMAKE_COMMAND} -E copy_directory
# "${PROJECT_BINARY_DIR}/shaders"
# "${CMAKE_SOURCE_DIR}/shaders"
# )