# find-imgui.cmake

find_package(imgui QUIET)

if (NOT imgui_FOUND)
    message(STATUS "Imgui not found, fetching from GitHub...")
    FetchContent_Declare(imgui_src
            URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9b.tar.gz
            #            EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(imgui_src)
    message(STATUS "Imgui src dir: ${imgui_src_SOURCE_DIR}")

    add_library(imgui
            ${imgui_src_SOURCE_DIR}/imgui.cpp
            ${imgui_src_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_src_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_src_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_src_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
            ${imgui_src_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    )
    target_include_directories(imgui
            PUBLIC ${imgui_src_SOURCE_DIR}
            PUBLIC ${imgui_src_SOURCE_DIR}/backends
    )
    message(STATUS "Imgui setup complete.")
else ()
    message(STATUS "Imgui already installed. Using version: ${imgui_VERSION}")
endif ()



