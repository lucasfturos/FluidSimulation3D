#pragma once

#include <GL/glew.h>
#include <stdexcept>
#include <vector>

class Texture {
  private:
    GLuint m_RendererID;
    GLenum m_TextureType;

  public:
    Texture(int width, int height, int depth, GLenum format, GLenum type,
            GLenum textureType)
        : m_RendererID(0), m_TextureType(textureType) {
        glGenTextures(1, &m_RendererID);
        glBindTexture(m_TextureType, m_RendererID);

        switch (m_TextureType) {
        case GL_TEXTURE_2D:
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                         type, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case GL_TEXTURE_3D:
            glTexImage3D(GL_TEXTURE_3D, 0, format, width, height, depth, 0,
                         format, type, nullptr);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        default:
            glDeleteTextures(1, &m_RendererID);
            m_RendererID = 0;
            throw std::invalid_argument("Unsupported texture type");
            break;
        }
        
        unbind();
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
            m_TextureType = other.m_TextureType;
            other.m_RendererID = 0;
        }
        return *this;
    }

    void bind(unsigned int slot = 0) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(m_TextureType, m_RendererID);
    }

    void unbind() const { glBindTexture(m_TextureType, 0); }

    void updateData(const std::vector<float> &data, int width, int height,
                    int depth, GLenum format, GLenum type) const {
        bind();
        switch (m_TextureType) {
        case GL_TEXTURE_2D:
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type,
                            data.data());
            break;
        case GL_TEXTURE_3D:
            glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth,
                            format, type, data.data());
            break;
        default:
            break;
        }
        unbind();
    }
};
