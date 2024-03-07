# LGE3D Root
set(LGE3D_ROOT ${ROOT_DIR}/include/lge3d)

file(GLOB_RECURSE LGE3D_SRC_FILES
        ${LGE3D_ROOT}/*.cpp
        ${LGE3D_ROOT}/*.hpp
        ${LGE3D_ROOT}/*.h
)

set(LGE3D_INCLUDE_DIR
        ${LGE3D_ROOT}
        ${LGE3D_ROOT}/systems
)
