# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(EXTERNAL_HEADERS
    imgui/imgui.h
    imgui/imconfig.h
    imgui/imgui_internal.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/backends/imgui_impl_opengl3.h
    imgui/backends/imgui_impl_opengl3_loader.h
    imgui/backends/imgui_impl_sdl2.h
)

set(EXTERNAL_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_sdl2.cpp
)

find_package(SDL2 REQUIRED)

add_library(External_lib STATIC 
    ${EXTERNAL_HEADERS}
    ${EXTERNAL_SOURCES}
)

foreach(SOURCE_FILE ${EXTERNAL_SOURCES})
    set_source_files_properties(${SOURCE_FILE} PROPERTIES COMPILE_FLAGS -w)
endforeach()

target_include_directories(External_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${SDL2_INCLUDE_DIRS}
)

# -------------------------------------------------------------------
