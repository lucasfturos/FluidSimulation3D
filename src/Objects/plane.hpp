#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

static const std::vector<glm::vec3> planeVertices = {
    {-1.0f, 0.0f, -1.0f},
    {1.0f, 0.0f, -1.0f},
    {1.0f, 0.0f, 1.0f},
    {-1.0f, 0.0f, 1.0f},
};

static const std::vector<GLuint> planeIndices = {
    0, 1, 2, 2, 3, 0,
};
