#include "vertex_buffer_layout.hpp"

template <typename T> void VertexBufferLayout::push(GLuint /* count */) {
    static_assert(t_always_false<T>(), "Type error");
}

template <> void VertexBufferLayout::push<GLfloat>(GLuint count) {
    m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template <> void VertexBufferLayout::push<GLuint>(GLuint count) {
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template <> void VertexBufferLayout::push<GLubyte>(GLuint count) {
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}
