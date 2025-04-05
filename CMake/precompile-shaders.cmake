# Compile GLSL Shaders into SPIR-V

# GlslValidator setup (Manually set this in your environment)
if(WIN32)   # Windows
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
elseif(APPLE)   # MacOS
    set(GLSL_VALIDATOR "$ENV{HOME}/.dev/VulkanSDK/1.4.309.0/macOS/bin/glslangValidator")
elseif(UNIX)    # Linux
    set(GLSL_VALIDATOR "/bin/glslangValidator")
else()  # Unsupported platform
    message(FATAL_ERROR "Unsupported platform for glslangValidator")
endif()
message(STATUS "Using glslangValidator from Vulkan SDK: ${GLSL_VALIDATOR}")

file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${SHA_DIR}/*.frag"
        "${SHA_DIR}/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${SPIRV_OUT_DIR}/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SPIRV_OUT_DIR}
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(PreCompileShaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

message(DEBUG "SPIRV files have been written to ${SPIRV_OUT_DIR}")

#if(MSVC)
#    add_custom_command(TARGET CompileShaders POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/Shaders
#            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SPIRV_OUT_DIR} ${PROJECT_BINARY_DIR}/$<CONFIG>/Shaders
#            DEPENDS CompileShaders
#    )
#endif(MSVC)
