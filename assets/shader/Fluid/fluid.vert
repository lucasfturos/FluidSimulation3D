#version 330 core

layout(location = 0) in vec3 aPos;
out vec3 TexCoords;
out mat3 NormalMatrix;

uniform mat4 uMVP;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

void main() {
    mat3 modelMat3 = mat3(uModelMatrix);
    mat3 viewMat3 = mat3(uViewMatrix);
    NormalMatrix = transpose(inverse(modelMat3)) * viewMat3;

    TexCoords = 0.5 * (aPos + 1.0);
    gl_Position = uMVP * vec4(aPos, 1.0);
}
