#version 330 core

in vec3 FragPos;
in vec3 TexCoords;
out vec4 color;

uniform mat4 uModel;
uniform sampler3D uDensity;
uniform vec3 uCameraPosition;
uniform vec3 uWorldParticlePosition;

const float stepSize = 0.005;
const int MAX_STEPS = 256;
const float EPSILON = 1.0e-5;
const float opacityFactor = 0.4;

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

vec3 hsv2rgb(float h, float s, float v) {
    float c = v * s;
    float x = c * (1.0 - abs(mod(h / 60.0, 2.0) - 1.0));
    float m = v - c;
    
    vec3 rgb;
    if (h >= 0.0 && h < 60.0) {
        rgb = vec3(c, x, 0.0);
    } else if (h >= 60.0 && h < 120.0) {
        rgb = vec3(x, c, 0.0);
    } else if (h >= 120.0 && h < 180.0) {
        rgb = vec3(0.0, c, x);
    } else if (h >= 180.0 && h < 240.0) {
        rgb = vec3(0.0, x, c);
    } else if (h >= 240.0 && h < 300.0) {
        rgb = vec3(x, 0.0, c);
    } else {
        rgb = vec3(c, 0.0, x);
    }
    return rgb + m;
}

void accumulateColorAndAlpha(inout vec4 accumulatedColor,
                             inout float accumulatedAlpha, float density, vec3 pos) {
    if (density > EPSILON) {
        float distanceToParticle = length(pos - uWorldParticlePosition);
        float influence = clamp(1.0 - distanceToParticle, 0.0, 1.0);

        float hue = mod(density * 360.0, 360.0);
        float saturation = 1.0 - (density * 0.5);
        float brightness = max(0.4, density);  
        vec3 colorSample = hsv2rgb(hue, saturation, brightness) * (0.8 + 0.2 * influence);

        float alpha = density * opacityFactor;
        accumulatedColor.rgb += (1.0 - accumulatedAlpha) * colorSample * alpha;
        accumulatedAlpha += (1.0 - accumulatedAlpha) * alpha;
    }
}

vec4 rayMarching(vec3 rayOrigin, vec3 rayDirection) {
    vec4 accumulatedColor = vec4(0.0);
    float accumulatedAlpha = 0.0;

    float tNear, tFar;
    if (!intersectBox(rayOrigin, rayDirection, vec3(1.4), tNear, tFar)) {
        return vec4(0.0);
    }

    float t = max(tNear, 0.0);
    for (int i = 0; i < MAX_STEPS; ++i) {
        if (t > tFar || accumulatedAlpha >= 1.0) break; 

        vec3 pos = rayOrigin + t * rayDirection;
        float density = sampleDensity(pos);
        accumulateColorAndAlpha(accumulatedColor, accumulatedAlpha, density, pos);

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
