#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
out vec3 TexCoords;

uniform mat4 uMVP;

void main() {
    TexCoords = (aPos + vec3(1.0)) / vec3(2.0);;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform sampler3D uDensity;

void main() {
    vec3 adjustedTexCoords = clamp(TexCoords, vec3(0.1), vec3(0.9));
    vec3 densityValue = texture(uDensity, adjustedTexCoords).rgb;

    float densityLength = length(densityValue);
    float alpha = smoothstep(0.0, 0.1, densityLength);

    if (densityLength < 0.01) {
        densityValue = vec3(0.0);
    }

    color = vec4(densityValue, alpha);
}
