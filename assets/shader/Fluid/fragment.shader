#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform sampler3D uDensity;

void main() {
    vec3 adjustedTexCoords = clamp(TexCoords, vec3(0.1), vec3(0.9));
    vec3 densityValue = texture(uDensity, adjustedTexCoords).rgb;

    float densityLength = length(densityValue);
    float alpha = smoothstep(0.0, 0.1, densityLength);

    if (densityLength < 0.01) densityValue = vec3(0.0);
    
    color = vec4(densityValue, alpha);
}