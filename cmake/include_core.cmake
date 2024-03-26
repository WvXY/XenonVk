# LGECore
set(LGECore_ROOT ${SRC_DIR}/Core)  # Maybe use CORE instead of Common

file(GLOB_RECURSE LGECore_SRC_FILES
        ${LGECore_ROOT}/*.cpp
        ${LGECore_ROOT}/*.hpp
        ${LGECore_ROOT}/*.h
)

set(LGECore_INCLUDE_DIR
        ${LGECore_ROOT}
)
