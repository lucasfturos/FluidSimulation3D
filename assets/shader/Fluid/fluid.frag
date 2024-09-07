#version 330 core

in vec3 TexCoords;
in mat3 NormalMatrix;
out vec4 color;

uniform sampler3D uDensity;

struct Ray {
    vec3 orig;
    vec3 dir;
};

struct AABB {
    vec3 top;
    vec3 bottom;
};

const float step_length = 0.005;

vec3 calculateNormal(vec3 pos, float intensity) {
    float d = step_length;
    float dx = texture(uDensity, pos + vec3(d, 0, 0)).r - intensity;
    float dy = texture(uDensity, pos + vec3(0, d, 0)).r - intensity;
    float dz = texture(uDensity, pos + vec3(0, 0, d)).r - intensity;
    return -normalize(NormalMatrix * vec3(dx, dy, dz));
}

void rayBoxIntersection(Ray ray, AABB box, out float t_0, out float t_1) {
    vec3 direction_inv = 1.0 / ray.dir;
    vec3 t_top = direction_inv * (box.top - ray.orig);
    vec3 t_bottom = direction_inv * (box.bottom - ray.orig);

    vec3 t_min = min(t_top, t_bottom);
    vec2 t = max(t_min.xx, t_min.yz);
    t_0 = max(0.0, max(t.x, t.y));

    vec3 t_max = max(t_top, t_bottom);
    t = min(t_max.xx, t_max.yz);
    t_1 = min(t.x, t.y);
}

void main() {
    float intensity = texture(uDensity, TexCoords).r;
    vec3 normal = calculateNormal(TexCoords, intensity);

    color = vec4(normal * 0.5 + 0.5, 1.0);
}
