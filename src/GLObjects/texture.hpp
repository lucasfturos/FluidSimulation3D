#pragma once

#include <GL/glew.h>
#include <vector>

class Texture {
  private:
    GLuint m_RendererID;

  public:
    Texture(int width, int height, int depth, GLenum format, GLenum type)
        : m_RendererID(0) {
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_3D, m_RendererID);
        glTexImage3D(GL_TEXTURE_3D, 0, format, width, height, depth, 0, format,
                     type, nullptr);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_3D, 0);
    }

    ~Texture() { glDeleteTextures(1, &m_RendererID); }

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    Texture(Texture &&other) noexcept : m_RendererID(other.m_RendererID) {
        other.m_RendererID = 0;
    }

    Texture &operator=(Texture &&other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }
        return *this;
    }

    void bind(unsigned int slot = 0) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_3D, m_RendererID);
    }

    void unbind() const { glBindTexture(GL_TEXTURE_3D, 0); }

    void updateData(const std::vector<float> &data, int width, int height,
                    int depth, GLenum format, GLenum type) const {
        bind();
        glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, format,
                        type, data.data());
        unbind();
    }
};
