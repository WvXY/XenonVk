# Check if GLM is already installed
find_package(glm QUIET)

if (NOT glm_FOUND)
    message(STATUS "GLM not found, fetching from GitHub...")
    FetchContent_Declare(glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(glm)
else ()
    message(STATUS "GLM already installed. Using version: ${glm_VERSION}")
endif ()

message(STATUS "GLM src dir: ${glm_SOURCE_DIR}")