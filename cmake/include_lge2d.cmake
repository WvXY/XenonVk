# LGE2D Root
set(LGE2D_ROOT ${ROOT_DIR}/include/lge2d)

file(GLOB_RECURSE LGE2D_SRC_FILES
        ${LGE2D_ROOT}/*.cpp
        ${LGE2D_ROOT}/*.hpp
        ${LGE2D_ROOT}/*.h
)

set(LGE2D_INCLUDE_DIR ${LGE2D_ROOT})
