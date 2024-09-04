#pragma once

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"

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

    template <typename T>
    void addBuffer(const VertexBuffer<T> &vb,
                   const VertexBufferLayout &layout) const {
        bind();
        vb.bind();
        const auto &elements = layout.getElements();
        GLuint offset = 0;
        GLuint index = 0;
        for (const auto &element : elements) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.count, element.type,
                                  element.normalized, layout.getStride(),
                                  reinterpret_cast<const void *>(offset));
            offset += element.count *
                      VertexBufferElement::getSizeOfType(element.type);
            ++index;
        }
        vb.unbind();
        unbind();
    }

    void bind() const { glBindVertexArray(m_RendererID); }

    void unbind() const { glBindVertexArray(0); }
};
