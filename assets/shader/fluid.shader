#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
out vec3 vPos;
out vec3 TexCoords;

uniform mat4 uMVP;

void main() {
    TexCoords = (aPos + 1.0) / 2.0;
    gl_Position = uMVP * vec4(aPos, 1.0);
    vPos = gl_Position.xyz / gl_Position.w;
}

#shader fragment
#version 330 core

#define STEPS 64
#define STEP_SIZE 1.0e-3
#define EPS 1.0e-6
#define MAX_DIST 50.0

in vec3 TexCoords;
in vec3 vPos;
out vec4 color;

uniform sampler3D densityTex;

// Função de distância para um cubo
float sdBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

// Função de distância para o fluido
float sdFluid(vec3 p) { 
    return texture(densityTex, p).r - 0.5; 
}

// Operações booleanas suaves
float opSmoothIntersection(float d1, float d2, float k) {
    float h = max(k - abs(d1 - d2), 0.0);
    return max(d1, d2) + h * h * 0.25 / k;
}

bool rayMarchHit(vec3 ro, vec3 rd, out vec3 hitPos) {
    float t = 0.0;
    float d = 1e10;

    for (int i = 0; i < STEPS; ++i) {
        vec3 p = ro + t * rd;
        float d1 = sdFluid(p);
        float d2 = sdBox(p, vec3(1.0));
        float dt = opSmoothIntersection(d1, d2, 0.25);
        d = min(d, dt);

        if (d < EPS) {
            hitPos = p;
            return true;
        }

        t += d * STEP_SIZE;
        if (t > MAX_DIST) {
            break;
        }
    }
    return false;
}

void main() {
    vec3 ro = TexCoords;
    vec3 rd = normalize(TexCoords - vPos);
    vec3 hitPos;

    if (rayMarchHit(ro, rd, hitPos)) {
        vec3 densityColor = texture(densityTex, hitPos).rgb;
        color = vec4(densityColor, 1.0);
    } else {
        discard;
    }
}
