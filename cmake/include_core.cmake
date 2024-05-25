# XEVVCore
set(XELCore_ROOT ${SRC_DIR}/Core)  # Maybe use CORE instead of Common

file(GLOB_RECURSE XEVCore_SRC_FILES
        ${XEVCore_ROOT}/*.cpp
        ${XEVCore_ROOT}/*.hpp
        ${XEVCore_ROOT}/*.h
)

set(XEVCore_INCLUDE_DIR
        ${XEVCore_ROOT}
)
