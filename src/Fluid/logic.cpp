#include "fluid.hpp"

void Fluid::step() {
    float dt = params.dt;
    float viscosity = params.viscosity;
    float diffusion = params.diffusion;

    // for (int i = 0; i < nSize; ++i) {
    //     Vx[i] += gravity.x * dt;
    //     Vy[i] += gravity.y * dt;
    //     Vz[i] += gravity.z * dt;
    // }

    diffuse(1, Vx0, Vx, viscosity, dt);
    diffuse(2, Vy0, Vy, viscosity, dt);
    diffuse(3, Vz0, Vz, viscosity, dt);

    project(Vx0, Vy0, Vz0, Vx, Vy);

    advect(1, Vx, Vx0, Vx0, Vy0, Vz0, dt);
    advect(2, Vy, Vy0, Vx0, Vy0, Vz0, dt);
    advect(3, Vz, Vz0, Vx0, Vy0, Vz0, dt);

    project(Vx, Vy, Vz, Vx0, Vy0);

    diffuse(0, s, density, diffusion, dt);
    advect(0, density, s, Vx, Vy, Vz, dt);

    fadeDensity();
}

void Fluid::setupFluidDynamics() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50, 350);

    for (const auto &vertex : cubeVertices) {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                for (int k = -1; k <= 1; ++k) {
                    addDensity(vertex + glm::vec3(i, j, k), dis(gen));
                }
            }
        }

        float noiseScale = 0.5f;
        for (auto i = 0; i < 8; ++i) {
            float noiseValue = perlin->noise({
                vertex.x * noiseScale,
                vertex.y * noiseScale,
                vertex.z * noiseScale,
            });
            float angle = noiseValue * 2.0f * M_PI;
            float vX = std::cos(angle) * 0.1f;
            float vY = std::sin(angle) * 0.1f;
            addVelocity(vertex, {vX, vY, 1.0f});
            addTurbulence(vertex, t, {vX, vY, 1.0f});
        }
    }
}

void Fluid::addDensity(glm::ivec3 pos, float amount) {
    int index = IX(pos.x, pos.y, pos.z);
    density[index] += amount;
}

void Fluid::addVelocity(glm::ivec3 pos, glm::vec3 amount) {
    int index = IX(pos.x, pos.y, pos.z);
    Vx[index] += amount.x;
    Vy[index] += amount.y;
    Vz[index] += amount.z;
}

void Fluid::addTurbulence(glm::ivec3 pos, float t, glm::vec3 amount) {
    int index = IX(pos.x, pos.y, pos.z);
    float noiseScale = 0.9f;
    float noiseWeight = 0.9f;
    float turbulenceStrength = 0.9f;

    float nx = 2 * static_cast<float>(pos.x) * noiseScale;
    float ny = 2 * static_cast<float>(pos.y) * noiseScale;
    float nz = 2 * static_cast<float>(pos.z) * noiseScale;

    float noiseValueX = perlin->noise(glm::vec3(nx, t, 0)) +
                        noiseWeight * perlin->noise(glm::vec3(nx, 2 * t, 0));
    float noiseValueY = perlin->noise(glm::vec3(ny, t, 0)) +
                        noiseWeight * perlin->noise(glm::vec3(ny, 2 * t, 0));
    float noiseValueZ = perlin->noise(glm::vec3(nz, t, 0)) +
                        noiseWeight * perlin->noise(glm::vec3(nz, 2 * t, 0));

    Vx[index] += amount.x * (1.0f + turbulenceStrength * noiseValueX);
    Vy[index] += amount.y * (1.0f + turbulenceStrength * noiseValueY);
    Vz[index] += amount.z * (1.0f + turbulenceStrength * noiseValueZ);
}

void Fluid::fadeDensity() {
    float fadeRate = params.fadeRate;
    for (std::size_t i = 0; i < density.size(); ++i) {
        density[i] = std::max(density[i] - fadeRate, 0.0f);
    }
}
