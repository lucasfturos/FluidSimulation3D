#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 uMVP;

out vec3 FragPos;

void main() {
    gl_Position = uMVP * vec4(position, 1.0);
    FragPos = position;
};

#shader fragment
#version 330 core

in vec3 FragPos;

out vec4 color;

const vec3 cameraPos = vec3(0.0, 0.0, 4.2);
const vec3 lightPos = vec3(0.0, 10.0, 0.0);
const vec3 lightDir = normalize(lightPos - cameraPos);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const vec3 objectColor = vec3(0.2, 0.3, 0.8);

const float ambientStrength = 0.3;
const float diffuseStrength = 1.0;

vec3 calculateLighting(vec3 normal, vec3 lightDir) {
    float diffuse = max(dot(normal, lightDir), 0.0);
    return vec3(0.7) * (ambientStrength + diffuseStrength * diffuse);
}

void main() {
    vec3 norm = normalize(FragPos);
    vec3 lighting = calculateLighting(norm, lightDir);
    vec3 result = lighting * objectColor;
    color = vec4(result, 1.0);
}