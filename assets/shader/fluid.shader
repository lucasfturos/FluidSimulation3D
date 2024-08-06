#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uMVP;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

out vec4 color;

void main() {
    color = vec4(0.2, 0.2, 0.2, 1.0);
}
