#version 330 core

in vec3 aPos;
out vec3 TexCoords;

uniform mat4 uMVP;

void main() {
    TexCoords = aPos * 0.5 + 0.5;
    gl_Position = uMVP * vec4(aPos, 1.0);
}
