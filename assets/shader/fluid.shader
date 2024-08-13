#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
out vec3 TexCoords;

uniform mat4 uMVP;

void main() {
    TexCoords = (aPos + 1.0) / 2.0;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform sampler3D uDensity;

void main() {
    vec3 densityValue = texture(uDensity, TexCoords).rgb;
    color = vec4(densityValue, 1.0);
}
