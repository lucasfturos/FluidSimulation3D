#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

static const std::vector<glm::vec3> cubeVertices = {
    // Front
    {-1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f},
    // Back
    {-1.0f, -1.0f, -1.0f},
    {-1.0f, 1.0f, -1.0f},
    {1.0f, 1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    // Left
    {-1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f},
    // Right
    {1.0f, -1.0f, -1.0f},
    {1.0f, 1.0f, -1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f},
    // Top
    {-1.0f, 1.0f, -1.0f},
    {-1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, -1.0f},
    // Bottom
    {-1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f}};

static const std::vector<GLuint> cubeIndices = {
    // Front
    0, 1, 2, 2, 3, 0,
    // Back
    4, 5, 6, 6, 7, 4,
    // Left
    8, 9, 10, 10, 11, 8,
    // Right
    12, 13, 14, 14, 15, 12,
    // Top
    16, 17, 18, 18, 19, 16,
    // Bottom
    20, 21, 22, 22, 23, 20};
