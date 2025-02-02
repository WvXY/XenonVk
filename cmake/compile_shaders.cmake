# Compile GLSL shaders into SPIR-V

if(WIN32)
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
elseif(UNIX)
    set(GLSL_VALIDATOR "/bin/glslangValidator")
endif()

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

add_custom_target(CompileShaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

message(DEBUG "SPIRV files have been written to ${SPIRV_OUT_DIR}")

#if(MSVC)
#    add_custom_command(TARGET CompileShaders POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/shaders
#            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SPIRV_OUT_DIR} ${PROJECT_BINARY_DIR}/$<CONFIG>/shaders
#            DEPENDS CompileShaders
#    )
#endif(MSVC)
