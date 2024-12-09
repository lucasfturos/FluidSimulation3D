#version 330 core

in vec3 aPos;
out vec3 FragPos;
out vec3 TexCoords;

uniform mat4 uMVP;
uniform mat4 uModel;

void main() {
    TexCoords = (uModel * vec4(aPos, 1.0)).xyz;
    gl_Position = uMVP * vec4(aPos, 1.0);
    FragPos = aPos * 0.5 + 0.5;
}
