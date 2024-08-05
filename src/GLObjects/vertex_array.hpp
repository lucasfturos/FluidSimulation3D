#pragma once

#include "VertexBufferLayout/vertex_buffer_layout.hpp"
#include "vertex_buffer.hpp"

class VertexArray {
  private:
    GLuint m_RendererID;

  public:
    VertexArray() : m_RendererID(0) { glGenVertexArrays(1, &m_RendererID); }

    ~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    VertexArray(VertexArray &&other) noexcept
        : m_RendererID(other.m_RendererID) {
        other.m_RendererID = 0;
    }

    VertexArray &operator=(VertexArray &&other) noexcept {
        if (this != &other) {
            glDeleteVertexArrays(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }
        return *this;
    }

    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
        bind();
        vb.bind();
        const auto &elements = layout.getElements();
        GLuint offset = 0;
        for (GLuint i = 0; i < elements.size(); ++i) {
            const auto &element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type,
                                  element.normalized, layout.getStride(),
                                  reinterpret_cast<const void *>(offset));
            offset += element.count *
                      VertexBufferElement::getSizeOfType(element.type);
        }
    }

    void bind() const { glBindVertexArray(m_RendererID); }

    void unbind() const { glBindVertexArray(0); }
};
