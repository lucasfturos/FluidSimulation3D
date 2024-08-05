#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

class Texture {
  private:
    GLuint m_rendererID;
    std::string m_filePath;
    GLubyte *m_localBuffer;
    GLint m_width, m_height, m_depth, m_BPP;
    bool m_is3D;

  public:
    Texture(const std::string &path);
    Texture(const std::vector<GLubyte> &volumeData, GLint width, GLint height,
            GLint depth);

    ~Texture();

    void bind(GLuint slot = 0U) const;
    void unbind() const;

    inline GLint getWidth() const { return m_width; }
    inline GLint getHeight() const { return m_height; }
    inline GLint getDepth() const { return m_depth; }
};
