#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uMVP;

void main() { 
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform sampler3D densityTex;

void main() {
    vec3 density = texture(densityTex, TexCoords).rgb;
    color = vec4(density, 1.0);
}
