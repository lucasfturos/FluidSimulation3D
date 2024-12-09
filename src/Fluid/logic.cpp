#include "fluid.hpp"

void Fluid::step() {
    float dt = m_SimulParams.dt;
    float viscosity = m_SimulParams.viscosity;
    float diffusion = m_SimulParams.diffusion;
    bool activeGravity = m_SimulParams.activeGravity;
    float gravityScale = m_SimulParams.gravityScale;

    if (activeGravity) {
        for (int i = 0; i < m_nSize; ++i) {
            m_Vx[i] += m_Gravity.x * dt * gravityScale;
            m_Vy[i] += m_Gravity.y * dt * gravityScale;
            m_Vz[i] += m_Gravity.z * dt * gravityScale;
        }
    }

    diffuse(1, m_Vx0, m_Vx, viscosity, dt);
    diffuse(2, m_Vy0, m_Vy, viscosity, dt);
    diffuse(3, m_Vz0, m_Vz, viscosity, dt);

    project(m_Vx0, m_Vy0, m_Vz0, m_Vx, m_Vy);

    advect(1, m_Vx, m_Vx0, m_Vx0, m_Vy0, m_Vz0, dt);
    advect(2, m_Vy, m_Vy0, m_Vx0, m_Vy0, m_Vz0, dt);
    advect(3, m_Vz, m_Vz0, m_Vx0, m_Vy0, m_Vz0, dt);

    project(m_Vx, m_Vy, m_Vz, m_Vx0, m_Vy0);

    diffuse(0, m_TempDensity, m_Density, diffusion, dt);
    advect(0, m_Density, m_TempDensity, m_Vx, m_Vy, m_Vz, dt);

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
            float noiseValue = m_PerlinNoise->noise({
                vertex.x * noiseScale,
                vertex.y * noiseScale,
                vertex.z * noiseScale,
            });
            float angle = noiseValue * 2.0f * M_PI;
            float vX = std::cos(angle) * 0.1f;
            float vY = std::sin(angle) * 0.1f;
            addVelocity(vertex, {vX, vY, 1.0f});
            addTurbulence(vertex, m_Time, {vX, vY, 1.0f});
        }
    }
}

void Fluid::addDensity(glm::ivec3 pos, float amount) {
    if (m_SimulParams.flowPaused)
        return;

    int index = IX(pos.x, pos.y, pos.z);
    m_Density[index] += amount;
}

void Fluid::addVelocity(glm::ivec3 pos, glm::vec3 amount) {
    if (m_SimulParams.flowPaused)
        return;

    int index = IX(pos.x, pos.y, pos.z);
    m_Vx[index] += amount.x;
    m_Vy[index] += amount.y;
    m_Vz[index] += amount.z;
}

void Fluid::addTurbulence(glm::ivec3 pos, float t, glm::vec3 amount) {
    int index = IX(pos.x, pos.y, pos.z);
    float noiseScale = 0.9f;
    float noiseWeight = 0.9f;
    float turbulenceStrength = 0.9f;

    float nx = 2 * static_cast<float>(pos.x) * noiseScale;
    float ny = 2 * static_cast<float>(pos.y) * noiseScale;
    float nz = 2 * static_cast<float>(pos.z) * noiseScale;

    float noiseValueX =
        m_PerlinNoise->noise(glm::vec3(nx, t, 0)) +
        noiseWeight * m_PerlinNoise->noise(glm::vec3(nx, 2 * t, 0));
    float noiseValueY =
        m_PerlinNoise->noise(glm::vec3(ny, t, 0)) +
        noiseWeight * m_PerlinNoise->noise(glm::vec3(ny, 2 * t, 0));
    float noiseValueZ =
        m_PerlinNoise->noise(glm::vec3(nz, t, 0)) +
        noiseWeight * m_PerlinNoise->noise(glm::vec3(nz, 2 * t, 0));

    m_Vx[index] += amount.x * (1.0f + turbulenceStrength * noiseValueX);
    m_Vy[index] += amount.y * (1.0f + turbulenceStrength * noiseValueY);
    m_Vz[index] += amount.z * (1.0f + turbulenceStrength * noiseValueZ);
}

void Fluid::fadeDensity() {
    float fadeRate = m_SimulParams.fadeRate;
    for (std::size_t i = 0; i < m_Density.size(); ++i) {
        m_Density[i] = std::max(m_Density[i] - fadeRate, 0.0f);
    }
}

bool Fluid::checkCollisionObject(const glm::vec3 &particlePosition,
                                     const std::shared_ptr<Objects> &obj) {
    glm::vec3 objectPosition = obj->getPosition();

    switch (obj->getObjectType()) {
    case ObjectType::Sphere: {
        float radius = obj->getSphere()->getRadius();
        float distance = glm::length(particlePosition - objectPosition);
        return distance < radius;
    }
    default:
        return false;
    }
}

void Fluid::applyCollisionResponse(glm::ivec3 particlePos,
                                   const glm::vec3 &objectPosition,
                                   const std::shared_ptr<Objects> &obj) {
    int index = IX(particlePos.x, particlePos.y, particlePos.z);

    switch (obj->getObjectType()) {
    case ObjectType::Sphere: {
        float radius = obj->getSphere()->getRadius();
        float distance = glm::length(glm::vec3(particlePos) - objectPosition);

        if (distance < radius) {
            float force = (radius - distance) * 0.5f;
            glm::vec3 direction =
                glm::normalize(glm::vec3(particlePos) - objectPosition);
            m_Vx[index] = force * direction.x;
            m_Vy[index] = force * direction.y;
            m_Vz[index] = force * direction.z;
        }
        break;
    }
    default:
        break;
    }
}

void Fluid::detectCollisions(const std::shared_ptr<Objects> &obj) {
    if (!m_SimulParams.collisionDetection)
        return;
    if (checkCollisionObject(m_ParticlePosition, obj)) {
        std::cout << "Colision\n";
        applyCollisionResponse(m_ParticlePosition, obj->getPosition(), obj);
    }
}
