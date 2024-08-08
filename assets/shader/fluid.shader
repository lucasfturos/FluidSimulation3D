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

#define STEPS 64
#define STEP_SIZE 1.0e-3
#define EPS 1.0e-6
#define MAX_DIST 100.0

in vec3 TexCoords;
out vec4 color;

uniform sampler3D densityTex;

float sdFluid(vec3 p) { return texture(densityTex, p).r - 0.5; }

bool rayMarchHit(vec3 pos, vec3 direction, out vec3 hitPos) {
    float t = 0.0;

    for (int i = 0; i < STEPS; ++i) {
        vec3 p = pos + t * direction;
        float d = sdFluid(p);
        if (d < EPS) {
            hitPos = p;
            return true;
        }

        t += d * STEP_SIZE;
        if (t > MAX_DIST) {
            break;
        }
    }
    return false;
}

void main() {
    vec3 rayOrigin = TexCoords;
    vec3 rayDirection = normalize(vec3(0.0, 0.0, -1.0));
    vec3 hitPos;

    if (rayMarchHit(rayOrigin, rayDirection, hitPos)) {
        vec3 densityColor = texture(densityTex, hitPos).rgb;
        color = vec4(densityColor, 1.0);
    }
}
