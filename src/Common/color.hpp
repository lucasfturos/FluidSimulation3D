#pragma once

#include <glm/glm.hpp>

inline glm::vec3 HSV2RGB(float h, float s, float v) {
    glm::vec3 color;
    int i = static_cast<int>(std::floor(h * 6));
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i % 6) {
    case 0:
        color = glm::vec3(v, t, p);
        break;
    case 1:
        color = glm::vec3(q, v, p);
        break;
    case 2:
        color = glm::vec3(p, v, t);
        break;
    case 3:
        color = glm::vec3(p, q, v);
        break;
    case 4:
        color = glm::vec3(t, p, v);
        break;
    case 5:
        color = glm::vec3(v, p, q);
        break;
    default:
        color = glm::vec3(0.0f, 0.0f, 0.0f);
        break;
    }
    return color;
}

inline glm::vec3 getColorByValue(float value, float saturation = 0.8f,
                                 float brightness = 0.8f) {
    float hue = value / 255.0f;
    return HSV2RGB(hue, saturation, brightness);
}