#include "fluid.hpp"

Fluid::Fluid(glm::mat4 projection)
    : m_PerlinNoise(std::make_shared<Perlin>()), m_TempDensity(m_nSize, 0.0f),
      m_Density(m_nSize, 0.0f), m_Vx(m_nSize, 0.0f), m_Vy(m_nSize, 0.0f),
      m_Vz(m_nSize, 0.0f), m_Vx0(m_nSize, 0.0f), m_Vy0(m_nSize, 0.0f),
      m_Vz0(m_nSize, 0.0f), m_Colors(m_nSize * 3, 0.0f),
      m_ProjMattrix(projection), m_ModelMatrix(0),
      m_Gravity(glm::vec3(0, 9.81f, 0)), m_Time(0.0f) {}

void Fluid::setup() {
    std::string vertexSrc = "assets/shader/Fluid/vertex.shader";
    std::string fragmentSrc = "assets/shader/Fluid/fragment.shader";
    m_Mesh = std::make_shared<Mesh<glm::vec3>>(cubeVertices, cubeIndices,
                                               vertexSrc, fragmentSrc);
    m_Mesh->setup<GLfloat>({3});

    Mesh<glm::vec3>::UniformsMap uniforms;

    uniforms["uDensity"] = [](std::shared_ptr<Shader> shader) {
        shader->setUniform1i("uDensity", 0);
    };
    uniforms["uCameraPosition"] = [this](std::shared_ptr<Shader> shader) {
        glm::vec3 cameraPos = glm::vec3(glm::inverse(m_ViewMatrix)[3]);
        shader->setUniform3f("uCameraPosition", cameraPos);
    };

    m_Mesh->setUniforms(uniforms);

    auto texture =
        std::make_shared<Texture>(N, N, N, GL_RGB, GL_FLOAT, GL_TEXTURE_3D);
    m_Mesh->setTexture(texture);
}

void Fluid::drawDensity() {
    m_Colors.clear();
    for (int z = 0; z < N; ++z) {
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                m_ParticlePosition = glm::vec3(x, y, z);

                int index = IX(x, y, z);
                float d = m_Density[index];
                int colorIndex = index * 3;
                m_Colors[colorIndex + 0] = d;
                m_Colors[colorIndex + 1] = d;
                m_Colors[colorIndex + 2] = d;
            }
        }
    }

    m_Mesh->updateTexture(m_Colors, N, N, N);
}

void Fluid::run() {
    step();
    setupFluidDynamics();
    drawDensity();

    Mesh<glm::vec3>::UniformsMap uniforms;

    uniforms["uMVP"] = [this](std::shared_ptr<Shader> shader) {
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 scale(0.4f);
        model = glm::scale(model, scale);
        float angle = /* m_Time *  */ glm::radians(90.0f);
        model *= glm::rotate(glm::mat4(1.0f), angle, {1.0f, 1.0f, 0.0f});
        m_ModelMatrix = model;
        glm::mat4 mvp = m_ProjMattrix * m_ViewMatrix * model;
        shader->setUniformMat4f("uMVP", mvp);
    };
    uniforms["uModel"] = [this](std::shared_ptr<Shader> shader) {
        shader->setUniformMat4f("uModel", m_ModelMatrix);
    };
    uniforms["uWorldParticlePosition"] = [this](
                                             std::shared_ptr<Shader> shader) {
        glm::vec3 worldParticlePosition =
            glm::vec3(m_ModelMatrix * glm::vec4(m_ParticlePosition, 1.0f));
        shader->setUniform3f("uWorldParticlePosition", worldParticlePosition);
    };

    m_Mesh->setUniforms(uniforms);

    m_Mesh->draw();
}
