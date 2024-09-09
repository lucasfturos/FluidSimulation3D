#version 330 core

in vec3 TexCoords;
in mat3 NormalMatrix;
out vec4 color;

uniform sampler3D uDensity;

const float step_length = 0.001;

vec3 calculateNormal(vec3 pos) {
    float d = step_length;
    float intensity = texture(uDensity, TexCoords).r;
    float dx = texture(uDensity, pos + vec3(d, 0, 0)).r - intensity;
    float dy = texture(uDensity, pos + vec3(0, d, 0)).r - intensity;
    float dz = texture(uDensity, pos + vec3(0, 0, d)).r - intensity;
    return -normalize(NormalMatrix * vec3(dx, dy, dz));
}

void main() {
    vec3 normal = calculateNormal(TexCoords);
    color = vec4(normal * 0.5 + 0.5, 1.0);
}
