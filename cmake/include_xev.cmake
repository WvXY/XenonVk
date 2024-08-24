# XEV Root
set(XEV_INCLUDE_ROOT ${SRC_DIR}/)

set(XEV_INCLUDE_DIR
        ${XEV_INCLUDE_ROOT}
        ${XEV_INCLUDE_ROOT}/renderer/
        ${XEV_INCLUDE_ROOT}/renderer/systems/
        ${XEV_INCLUDE_ROOT}/common/
        ${XEV_INCLUDE_ROOT}/physics/
)

file(GLOB_RECURSE XEV_INCLUDE_FILES
        ${XEV_INCLUDE_DIR}/*.hpp
        ${XEV_INCLUDE_DIR}/*.h
)
