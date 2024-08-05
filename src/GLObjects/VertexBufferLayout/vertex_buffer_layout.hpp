#pragma once

#include <GL/glew.h>
#include <vector>

struct VertexBufferElement {
    GLuint type;
    GLuint count;
    GLubyte normalized;

    static GLuint getSizeOfType(GLuint type) {
        switch (type) {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        return 0;
    }
};

template <typename T> constexpr bool t_always_false() { return false; }

class VertexBufferLayout {
  private:
    std::vector<VertexBufferElement> m_elements;
    GLuint m_stride;

  public:
    VertexBufferLayout() : m_stride(0) {}

    template <typename T> void push(GLuint count);

    inline const std::vector<VertexBufferElement> getElements() const {
        return m_elements;
    }

    inline GLuint getStride() const { return m_stride; }
};
