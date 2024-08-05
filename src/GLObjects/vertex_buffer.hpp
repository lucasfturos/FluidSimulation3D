#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <vector>

class VertexBuffer {
  private:
    GLuint m_RendererID;

  public:
    VertexBuffer(const std::vector<glm::vec3> &data) : m_RendererID(0) {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * data.size(),
                     data.data(), GL_DYNAMIC_DRAW);
    }

    ~VertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

    VertexBuffer(const VertexBuffer &) = delete;
    VertexBuffer &operator=(const VertexBuffer &) = delete;

    VertexBuffer(VertexBuffer &&other) noexcept
        : m_RendererID(other.m_RendererID) {
        other.m_RendererID = 0;
    }

    VertexBuffer &operator=(VertexBuffer &&other) noexcept {
        if (this != &other) {
            glDeleteBuffers(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }
        return *this;
    }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    void updateData(const std::vector<glm::vec3> &data) const {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * data.size(),
                        data.data());
    }
};
