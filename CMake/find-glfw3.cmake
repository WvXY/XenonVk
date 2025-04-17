# Check if GLFW is already installed
find_package(glfw3 QUIET)

if (NOT glfw3_FOUND)
    message(STATUS "GLFW not found, fetching from GitHub...")
    FetchContent_Declare(glfw3
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG master
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    # set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland support in GLFW" FORCE)
    FetchContent_MakeAvailable(glfw3)
else ()
    message(STATUS "GLFW already installed. Using version: ${glfw3_VERSION}")
endif ()

message(STATUS "GLFW src dir: ${glfw3_SOURCE_DIR}")