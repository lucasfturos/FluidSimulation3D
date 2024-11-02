#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform mat4 uModel;
uniform sampler3D uDensity;
uniform vec3 uCameraPosition;

const float stepSize = 0.005;
const int MAX_STEPS = 256;
const float EPSILON = 1.0e-5;
const float opacityFactor = 0.3;

float sdBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sampleDensity(vec3 pos) {
    return texture(uDensity, clamp(pos * 0.5 + 0.5, 0.0, 1.0)).r;
}

bool intersectBox(vec3 rayOrigin, vec3 rayDir, vec3 boxSize, out float tNear, out float tFar) {
    vec3 invDir = 1.0 / rayDir;
    vec3 t0 = (vec3(-boxSize) - rayOrigin) * invDir;
    vec3 t1 = (vec3( boxSize) - rayOrigin) * invDir;
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);
    tNear = max(max(tmin.x, tmin.y), tmin.z);
    tFar = min(min(tmax.x, tmax.y), tmax.z);
    return tFar > max(tNear, 0.0);
}

void accumulateColorAndAlpha(inout vec4 accumulatedColor,
                             inout float accumulatedAlpha, float density) {
    if (density > EPSILON) {
        vec3 colorSample = vec3(density);
        float alpha = density * opacityFactor;
        accumulatedColor.rgb += (1.0 - accumulatedAlpha) * colorSample * alpha;
        accumulatedAlpha += (1.0 - accumulatedAlpha) * alpha;
    }
}

vec4 rayMarching(vec3 rayOrigin, vec3 rayDirection) {
    vec4 accumulatedColor = vec4(0.0);
    float accumulatedAlpha = 0.0;

    float tNear, tFar;
    if (!intersectBox(rayOrigin, rayDirection, vec3(1.0), tNear, tFar)) {
        return vec4(0.0);
    }

    float t = max(tNear, 0.0);
    for (int i = 0; i < MAX_STEPS; ++i) {
        if (t > tFar || accumulatedAlpha >= 1.0) break; 

        vec3 pos = rayOrigin + t * rayDirection;
        float density = sampleDensity(pos);
        accumulateColorAndAlpha(accumulatedColor, accumulatedAlpha, density);

        t += stepSize;
    }

    return vec4(accumulatedColor.rgb, accumulatedAlpha);
}

void main() {
    vec3 rayOrigin = vec3(inverse(uModel) * vec4(TexCoords, 1.0));
    vec3 newCameraPos = vec3(inverse(uModel) * vec4(uCameraPosition, 1.0));
    vec3 rayDirection = normalize(rayOrigin - newCameraPos);

    color = rayMarching(rayOrigin, rayDirection);
}
