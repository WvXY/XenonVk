# VKE Root
set(VKE_ROOT ${SRC_DIR}/vulkanEngine)

file(GLOB_RECURSE VKE_SRC_FILES
        ${VKE_ROOT}/*.cpp
        ${VKE_ROOT}/*.hpp
        ${VKE_ROOT}/*.h
)

set(VKE_INCLUDE_DIR
        ${VKE_ROOT}
        ${VKE_ROOT}/systems
)
