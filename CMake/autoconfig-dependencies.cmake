# Vulkan SDK
find_package(Vulkan REQUIRED)

if (Vulkan_FOUND)
    message(STATUS "Vulkan SDK found: ${Vulkan_INCLUDE_DIRS}")
    message(STATUS "Vulkan SDK Version: ${Vulkan_VERSION}")
    if (APPLE)
        set(CMAKE_MACOSX_RPATH ON)
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        set(CMAKE_INSTALL_RPATH /usr/local/lib)
    endif()
else ()
    message(FATAL_ERROR "Vulkan SDK not found")
endif ()

# Auto-setup dependencies
include(FetchContent)

# Check if GLFW is already installed
find_package(glfw3 QUIET)

if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found, fetching from GitHub...")
    FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    # set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland support in GLFW" FORCE)
    FetchContent_MakeAvailable(glfw)
else()
    message(STATUS "GLFW already installed. Using version: ${glfw3_VERSION}")
endif()

message(STATUS "GLFW include dir: ${glfw_SOURCE_DIR}/include")

# Check if GLM is already installed
find_package(glm QUIET)

if(NOT glm_FOUND)
    message(STATUS "GLM not found, fetching from GitHub...")
    FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG master)
    FetchContent_MakeAvailable(glm)
else()
    message(STATUS "GLM already installed. Using version: ${glm_VERSION}")
endif()

message(STATUS "GLM include dir: ${glm_SOURCE_DIR}/glm")
