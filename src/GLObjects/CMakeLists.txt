# -------------------------------------------------------------------

set(GLOBJECTS_HEADERS
    VertexBufferLayout/vertex_buffer_layout.hpp
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
