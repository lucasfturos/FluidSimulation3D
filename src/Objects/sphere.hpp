#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Sphere {
  public:
    Sphere(GLfloat radius, GLint slices) : radius(radius), slices(slices) {}

    std::vector<GLuint> genIndices() {
        for (GLint lat = 0; lat < slices; ++lat) {
            for (GLint lon = 0; lon < slices; ++lon) {
                GLuint first = lat * (slices + 1) + lon;
                GLuint second = first + slices + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
        return indices;
    }

    std::vector<glm::vec3> genVertices() {
        for (GLint lat = 0; lat <= slices; ++lat) {
            GLfloat theta = (lat * M_PI) / slices;
            GLfloat sinTheta = std::sin(theta);
            GLfloat cosTheta = std::cos(theta);

            for (GLint lon = 0; lon <= slices; ++lon) {
                GLfloat phi = (lon * 2 * M_PI) / slices;
                GLfloat sinPhi = std::sin(phi);
                GLfloat cosPhi = std::cos(phi);

                GLfloat x = cosPhi * sinTheta;
                GLfloat y = cosTheta;
                GLfloat z = sinPhi * sinTheta;

                vertices.push_back(glm::vec3(x, y, z) * radius);
            }
        }
        return vertices;
    }

  private:
    GLfloat radius;
    GLint slices;
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
};
