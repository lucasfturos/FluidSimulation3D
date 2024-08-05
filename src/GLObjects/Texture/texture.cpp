#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const std::string &path)
    : m_rendererID(0), m_filePath(path), m_localBuffer(nullptr), m_width(0),
      m_height(0), m_depth(1), m_BPP(0), m_is3D(false) {
    stbi_set_flip_vertically_on_load_thread(1);
    m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);

    glGenTextures(1, &m_rendererID);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, m_localBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_localBuffer) {
        stbi_image_free(m_localBuffer);
    }
}

Texture::Texture(const std::vector<GLubyte> &volumeData, GLint width,
                 GLint height, GLint depth)
    : m_rendererID(0), m_localBuffer(nullptr), m_width(width), m_height(height),
      m_depth(depth), m_BPP(1), m_is3D(true) {
    glGenTextures(1, &m_rendererID);
    glBindTexture(GL_TEXTURE_3D, m_rendererID);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, m_width, m_height, m_depth, 0,
                 GL_RED, GL_UNSIGNED_BYTE, volumeData.data());
    glBindTexture(GL_TEXTURE_3D, 0);
}

Texture::~Texture() { glDeleteTextures(1, &m_rendererID); }

void Texture::bind(GLuint slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    if (m_is3D) {
        glBindTexture(GL_TEXTURE_3D, m_rendererID);
    } else {
        glBindTexture(GL_TEXTURE_2D, m_rendererID);
    }
}

void Texture::unbind() const {
    if (m_is3D) {
        glBindTexture(GL_TEXTURE_3D, 0);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
