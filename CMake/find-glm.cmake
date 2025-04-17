# Check if GLM is already installed
find_package(glm QUIET)

if (NOT glm_FOUND)
    message(STATUS "GLM not found, fetching from GitHub...")
    file(MAKE_DIRECTORY ${EXT_DIR}/glm)
    FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG master
            #                        SOURCE_DIR ${EXT_DIR}/glm
    )
    FetchContent_MakeAvailable(glm)
else ()
    message(STATUS "GLM already installed. Using version: ${glm_VERSION}")
endif ()

message(STATUS "GLM include dir: ${glm_SOURCE_DIR}/glm")