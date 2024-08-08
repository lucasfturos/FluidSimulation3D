# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(EXTERNAL_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_sdl2.cpp
)

add_library(External_lib STATIC ${EXTERNAL_SOURCES})

foreach(SOURCE_FILE ${EXTERNAL_SOURCES})
    set_source_files_properties(${SOURCE_FILE} PROPERTIES COMPILE_FLAGS -w)
endforeach()

target_include_directories(External_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${SDL2_INCLUDE_DIRS}
)

# -------------------------------------------------------------------
