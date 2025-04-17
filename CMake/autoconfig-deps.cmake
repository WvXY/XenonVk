# Vulkan SDK
find_package(Vulkan REQUIRED)

if (Vulkan_FOUND)
    message(STATUS "Vulkan SDK found: ${Vulkan_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "Vulkan SDK not found")
endif ()

# Auto-setup dependencies
include(FetchContent)

## Problem: put all dependencies with different builds in the External folder causes issues with MakeAvailable
#set(FETCHCONTENT_BASE_DIR ${EXT_DIR})


include(${CMK_DIR}/find-glfw3.cmake)
include(${CMK_DIR}/find-glm.cmake)
include(${CMK_DIR}/find-imgui.cmake)


