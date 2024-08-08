# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(GLOBJECTS_HEADERS
    VertexBufferLayout/vertex_buffer_layout.hpp
    index_buffer.hpp
    shader.hpp
    texture.hpp
    vertex_array.hpp
    vertex_buffer.hpp
)

set(GLOBJECTS_SOURCES
    VertexBufferLayout/vertex_buffer_layout.cpp
)

add_library(GLObjects_lib 
    ${GLOBJECTS_HEADERS}
    ${GLOBJECTS_SOURCES}
)

target_include_directories(GLObjects_lib
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
