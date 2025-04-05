# Vulkan SDK
find_package(Vulkan REQUIRED)

if (Vulkan_FOUND)
    message(STATUS "Vulkan SDK found: ${Vulkan_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "Vulkan SDK not found")
endif ()

# Auto-setup dependencies
include(FetchContent)

FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

message(STATUS "GLFW include dir: ${glfw_SOURCE_DIR}/include")

# Math lib: GLM
FetchContent_Declare(glm GIT_REPOSITORY	https://github.com/g-truc/glm.git GIT_TAG master)
FetchContent_MakeAvailable(glm)
message(STATUS "GLM include dir: ${glm_SOURCE_DIR}/glm")