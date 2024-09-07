#include "fluid.hpp"
#include "Common/color.hpp"

Fluid::Fluid(glm::mat4 projection)
    : perlin(std::make_shared<Perlin>()), s(nSize, 0.0f), density(nSize, 0.0f),
      Vx(nSize, 0.0f), Vy(nSize, 0.0f), Vz(nSize, 0.0f), Vx0(nSize, 0.0f),
      Vy0(nSize, 0.0f), Vz0(nSize, 0.0f), colors(nSize * 3, 0.0f),
      viewMat(glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f),
                          glm::vec3(0.0f, 0.0f, 0.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f))),
      projMat(projection), gravity(glm::vec3(0, 9.81f, 0)), t(0.0f) {}

void Fluid::setTime(float time) { t = time; }

void Fluid::setSimulationParams(SimulationParams p) { params = p; }

void Fluid::drawDensity() {
    colors.clear();

    for (int z = 0; z < N; ++z) {
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                int index = IX(x, y, z);
                float d = density[index];
                auto color = getColorByValue(std::fmod((d + 50), 255.0f),
                                             100 / 255.0f, d / 255.0f);
                int colorIndex = index * 3;
                colors[colorIndex + 0] = color.r;
                colors[colorIndex + 1] = color.g;
                colors[colorIndex + 2] = color.b;
            }
        }
    }

    texture->updateData(colors, N, N, N, GL_RGB, GL_FLOAT);
}

void Fluid::setup() {
    va = std::make_shared<VertexArray>();
    vb = std::make_shared<VertexBuffer<glm::vec3>>(cubeVertices);
    ib = std::make_shared<IndexBuffer>(cubeIndices);

    VertexBufferLayout layout;
    layout.push<GLfloat>(3);
    va->addBuffer(*vb, layout);

    shader = std::make_shared<Shader>("assets/shader/Fluid/fluid.vert",
                                      "assets/shader/Fluid/fluid.frag");
    texture = std::make_shared<Texture>(N, N, N, GL_RGB, GL_FLOAT);

    shader->bind();
    shader->setUniform1i("uDensity", 0);
}

void Fluid::run() {
    step();

    setupFluidDynamics();

    drawDensity();

    shader->bind();

    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 scale(0.3f);
    model = glm::scale(model, scale);
    float angle = /* t * */ glm::radians(90.0f);
    model *= glm::rotate(glm::mat4(1.0f), angle, {1.0f, 1.0f, 0.0f});
    glm::mat4 mvp = projMat * viewMat * model;

    shader->setUniformMat4f("uMVP", mvp);
    shader->setUniformMat4f("uViewMatrix", viewMat);
    shader->setUniformMat4f("uModelMatrix", model);

    va->bind();
    vb->bind();
    ib->bind();
    texture->bind(0);

    glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, nullptr);

    va->unbind();
    vb->unbind();
    ib->unbind();
    shader->unbind();
    texture->unbind();
}
