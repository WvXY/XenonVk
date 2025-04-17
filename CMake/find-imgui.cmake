# find-imgui.cmake

find_package(imgui QUIET)

if (NOT imgui_FOUND)
    message(STATUS "Imgui not found, fetching from GitHub...")
    #file(MAKE_DIRECTORY ${EXT_DIR}/imgui)
    FetchContent_Declare(imgui
            URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9b.tar.gz
            EXCLUDE_FROM_ALL
            #        SOURCE_DIR ${EXT_DIR}/imgui
    )
    FetchContent_MakeAvailable(imgui)
else ()
    message(STATUS "Imgui already installed. Using version: ${imgui_VERSION}")
endif ()

message(STATUS "Imgui include dir: ${imgui_SOURCE_DIR}/glm")

#add_library(imgui
#        ${imgui_external_SOURCE_DIR}/imgui.cpp
#        ${imgui_external_SOURCE_DIR}/imgui_draw.cpp
#        ${imgui_external_SOURCE_DIR}/imgui_tables.cpp
#        ${imgui_external_SOURCE_DIR}/imgui_widgets.cpp
#        ${imgui_external_SOURCE_DIR}/backends/imgui_impl_allegro5.cpp
#)
#target_include_directories(imgui PUBLIC ${imgui_external_SOURCE_DIR})
