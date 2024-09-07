#version 330 core

in vec3 aPos;
out vec3 TexCoords;
out mat3 NormalMatrix;

uniform mat4 uMVP;
uniform mat3 uNormalMatrix;

void main() {
    NormalMatrix = uNormalMatrix;
    TexCoords = 0.5 * (aPos + 1.0);
    gl_Position = uMVP * vec4(aPos, 1.0);
}
