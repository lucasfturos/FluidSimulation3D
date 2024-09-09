#version 330 core

in vec3 FragPos;
out vec4 color;

const vec3 cameraPos = vec3(0.0, 2.0, 20.0);
const vec3 lightPos = vec3(5.0, 5.0, 1.5);
const vec3 lightDir = normalize(lightPos - cameraPos);
const vec3 objectColor = vec3(0.2, 0.3, 0.8);

const float ambientStrength = 0.2;
const float diffuseStrength = 0.6;

vec3 calculateLighting(vec3 pos) {
    float diffuse = max(dot(pos, lightDir), 0.0);
    return vec3(0.7) * (ambientStrength + diffuseStrength * diffuse);
}

void main() {
    vec3 lighting = calculateLighting(FragPos);
    vec3 result = lighting * objectColor;
    float gamma = 1.2;
    result = pow(result, vec3(1.0 / gamma));
    color = vec4(result, 1.0);
}