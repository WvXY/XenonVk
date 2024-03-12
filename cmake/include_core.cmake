# LGECore
set(LGECore_ROOT ${ROOT_DIR}/include/Core)  # Maybe use CORE instead of Common

file(GLOB_RECURSE LGECore_SRC_FILES
        ${LGECore_ROOT}/*.cpp
        ${LGECore_ROOT}/*.hpp
)

set(LGECore_INCLUDE_DIR
        ${LGECore_ROOT}
)
