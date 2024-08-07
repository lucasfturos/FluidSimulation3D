#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uMVP;
out vec3 TexCoords;

void main() {
    TexCoords = (aPos + 1.0) / 2.0;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform sampler3D densityTex;
uniform float iTime;

float sdBox(vec3 p) {
    vec3 d = abs(p) - vec3(1.0);
    return length(max(d, 0.0)) + min(max(d.x, max(d.y, d.z)), 0.0);
}

float opSmoothUnion(float d1, float d2, float k) {
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return mix(d2, d1, h) - k * h * (1.0 - h);
}

float sdFluid(vec3 p) { return texture(densityTex, p).r - 0.5; }

float QueryVolumetricDistanceField(vec3 pos) {
    float sdfBox = sdBox(pos);
    float sdfFluid = sdFluid(pos);
    return opSmoothUnion(sdfBox, sdfFluid, 0.4);
}

void main() {
    vec3 rayOrigin = TexCoords;
    vec3 rayDirection = normalize(vec3(0.0, 0.0, -1.0));

    float t = 0.0;
    const float maxDistance = 20.0;
    const float epsilon = 1.0e-6;
    const int maxSteps = 100;
    const float k = 0.4;

    for (int i = 0; i < maxSteps; ++i) {
        vec3 p = rayOrigin + t * rayDirection;

        float d = QueryVolumetricDistanceField(p);

        if (d < epsilon) {
            vec3 densityColor = texture(densityTex, p).rgb;
            color = vec4(densityColor, 1.0);
            return;
        }
        t += d;
        if (t > maxDistance) {
            break;
        }
    }

    color = vec4(0.0, 0.0, 0.0, 0.0);
}
