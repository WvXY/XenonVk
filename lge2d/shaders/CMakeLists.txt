# Compile GLSL shaders into SPIR-V

if(WIN32)
        set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
elseif(UNIX)
        set(GLSL_VALIDATOR "/bin/glslangValidator")
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
        "*.frag"
        "*.vert"
)

# maybe need to set another dir name for 2d shaders
foreach(GLSL ${GLSL_SOURCE_FILES})
        get_filename_component(FILE_NAME ${GLSL} NAME)
        set(SPIRV "${SHADER_SPIRV_OUTPUT_DIRECTORY}/${FILE_NAME}.spv")
        add_custom_command(
                OUTPUT ${SPIRV}
                COMMAND ${CMAKE_COMMAND} -E make_directory
                        ${SHADER_SPIRV_OUTPUT_DIRECTORY}
                COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
                DEPENDS ${GLSL})
        list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders2D
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(${PROJECT_NAME} Shaders2D)

if(MSVC) 
        add_custom_command(TARGET Shaders2D POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                        ${OUT_DIR}/$<CONFIG>/shaders
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${SHADER_SPIRV_OUTPUT_DIRECTORY}
                        ${OUT_DIR}/$<CONFIG>/shaders
                DEPENDS Shaders2D)
endif(MSVC)
