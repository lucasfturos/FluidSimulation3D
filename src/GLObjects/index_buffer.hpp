#pragma once

#include <GL/glew.h>
#include <vector>

class IndexBuffer {
  private:
    GLuint m_RendererID;

  public:
    IndexBuffer(const std::vector<GLuint> &data) : m_RendererID(0) {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint),
                     data.data(), GL_DYNAMIC_DRAW);
    }

    ~IndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

    IndexBuffer(const IndexBuffer &) = delete;
    IndexBuffer &operator=(const IndexBuffer &) = delete;

    IndexBuffer(IndexBuffer &&other) noexcept
        : m_RendererID(other.m_RendererID) {
        other.m_RendererID = 0;
    }

    IndexBuffer &operator=(IndexBuffer &&other) noexcept {
        if (this != &other) {
            glDeleteBuffers(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
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
};
