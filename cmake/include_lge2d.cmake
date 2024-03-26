# LGE2D Root
set(LGE2D_INCLUDE_DIR ${SRC_DIR}/lge2d)

file(GLOB_RECURSE LGE2D_SRC_FILES
        ${LGE2D_INCLUDE_DIR}/*.cpp
        ${LGE2D_INCLUDE_DIR}/*.hpp
        ${LGE2D_INCLUDE_DIR}/*.h
)

