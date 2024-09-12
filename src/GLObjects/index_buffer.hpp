#pragma once

#include <GL/glew.h>
#include <vector>

class IndexBuffer {
  private:
    GLuint m_RendererID;
    GLsizei m_Count;

  public:
    IndexBuffer(const std::vector<GLuint> &data)
        : m_RendererID(0), m_Count(static_cast<GLsizei>(data.size())) {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(GLuint),
                     data.data(), GL_DYNAMIC_DRAW);
    }

    ~IndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

    IndexBuffer(const IndexBuffer &) = delete;
    IndexBuffer &operator=(const IndexBuffer &) = delete;

    IndexBuffer(IndexBuffer &&other) noexcept
        : m_RendererID(other.m_RendererID) {
        other.m_RendererID = 0;
        other.m_Count = 0;
    }

    IndexBuffer &operator=(IndexBuffer &&other) noexcept {
        if (this != &other) {
            glDeleteBuffers(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
            other.m_Count = 0;
        }
        return *this;
    }

    void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }

    void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    void updateData(const std::vector<GLuint> &data) const {
        bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                        data.size() * sizeof(GLuint), data.data());
    }

    GLsizei getCount() const { return m_Count; }
};
