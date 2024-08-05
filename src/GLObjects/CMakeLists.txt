# -------------------------------------------------------------------

set(GLOBJECTS_HEADERS
    Texture/texture.hpp
    VertexBufferLayout/vertex_buffer_layout.hpp
)

set(GLOBJECTS_SOURCES
    Texture/texture.cpp
    VertexBufferLayout/vertex_buffer_layout.cpp
)

add_library(GLObjects_lib 
    ${GLOBJECTS_HEADERS}
    ${GLOBJECTS_SOURCES}
)

target_include_directories(GLObjects_lib 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
