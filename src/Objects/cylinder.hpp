#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Cylinder {
  public:
    Cylinder(GLfloat h, GLfloat radiusTop, GLfloat radiusBottom, GLint slices)
        : height(h), radiusTop(radiusTop), radiusBottom(radiusBottom),
          slices(slices) {}

    std::vector<GLuint> genIndices() {
        for (auto i = 0; i < slices; ++i) {
            GLuint current = i * 2;
            GLuint next = (i + 1) % (slices + 1) * 2;

            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(next + 1);
            indices.push_back(current + 1);
            indices.push_back(next);
        }

        const GLuint baseCenterIndex = vertices.size() / 3;
        const GLuint topCenterIndex = baseCenterIndex + slices + 1;

        for (auto i = 0; i < slices; ++i) {
            indices.push_back(i * 2);
            indices.push_back((i + 1) % slices * 2);
            indices.push_back(baseCenterIndex);
        }

        for (auto i = 0; i < slices; ++i) {
            indices.push_back((i + 1) % slices * 2 + 1);
            indices.push_back(i * 2 + 1);
            indices.push_back(topCenterIndex);
        }

        return indices;
    }

    std::vector<glm::vec3> genVertices() {
        const GLfloat angleIncrement = (2 * M_PI) / slices;
        for (auto i = 0; i <= slices; ++i) {
            GLfloat angle = i * angleIncrement;

            GLfloat topX = radiusTop * std::cos(angle);
            GLfloat topY = radiusTop * std::sin(angle);
            GLfloat topZ = height / 2.0f;
            vertices.push_back(glm::vec3(topX, topY, topZ));

            GLfloat bottomX = radiusBottom * std::cos(angle);
            GLfloat bottomY = radiusBottom * std::sin(angle);
            GLfloat bottomZ = -height / 2.0f;
            vertices.push_back(glm::vec3(bottomX, bottomY, bottomZ));
        }
        return vertices;
    }

  private:
    GLfloat height;
    GLfloat radiusTop;
    GLfloat radiusBottom;
    GLint slices;
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
};
