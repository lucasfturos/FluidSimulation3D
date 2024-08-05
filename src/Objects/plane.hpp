#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Plane {
  public:
    Plane(GLfloat w, GLfloat h, GLint numSeg)
        : width(w), height(h), numSegment(numSeg) {}

    std::vector<GLuint> genIndices() {
        for (GLint i = 0; i < numSegment; ++i) {
            for (GLint j = 0; j < numSegment; ++j) {
                GLuint topLeft = i + (j * (numSegment + 1));
                GLuint topRight = (i + 1) + (j * (numSegment + 1));
                GLuint bottomLeft = i + ((j + 1) * (numSegment + 1));
                GLuint bottomRight = (i + 1) + ((j + 1) * (numSegment + 1));

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
        return indices;
    }

    std::vector<glm::vec3> genVertices() {
        GLfloat halfWidth = width / 2.0f;
        GLfloat halfHeight = height / 2.0f;

        for (GLint i = 0; i <= numSegment; ++i) {
            GLfloat t = static_cast<GLfloat>(i) / numSegment;
            GLfloat x = t * width - halfWidth;
            for (GLint j = 0; j <= numSegment; ++j) {
                GLfloat s = static_cast<GLfloat>(j) / numSegment;
                GLfloat z = s * height - halfHeight;
                vertices.emplace_back(x, 0.0f, z);
            }
        }

        return vertices;
    }

  private:
    GLfloat width;
    GLfloat height;
    GLint numSegment;
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
};