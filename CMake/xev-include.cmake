# XEV Root
set(XEV_INCLUDE_ROOT ${SRC_DIR}/)

set(XEV_INCLUDE_DIR
        ${XEV_INCLUDE_ROOT}
        ${XEV_INCLUDE_ROOT}/Core/
        ${XEV_INCLUDE_ROOT}/Renderer/
        ${XEV_INCLUDE_ROOT}/Renderer/Systems/
        ${XEV_INCLUDE_ROOT}/Math/
        ${XEV_INCLUDE_ROOT}/Components/
        ${XEV_INCLUDE_ROOT}/Utils/
)

file(GLOB_RECURSE XEV_INCLUDE_FILES
        ${XEV_INCLUDE_DIR}/*.hpp
        ${XEV_INCLUDE_DIR}/*.h
)
