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

void Fluid::setup() {
    mesh = std::make_shared<Mesh>(cubeVertices, cubeIndices,
                                  "assets/shader/Fluid/vertex.shader",
                                  "assets/shader/Fluid/fragment.shader");
    mesh->setup<GLfloat>({3});

    Mesh::UniformsMap uniforms = {
        {"uDensity", [](std::shared_ptr<Shader> shader) {
             shader->setUniform1i("uDensity", 0);
         }}};
    mesh->setUniforms(uniforms);

    auto texture = std::make_shared<Texture>(N, N, N, GL_RGB, GL_FLOAT);
    mesh->setTexture(texture);
}

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

    mesh->updateTexture(colors, N, N, N);
}

void Fluid::run() {
    step();
    setupFluidDynamics();
    drawDensity();

    Mesh::UniformsMap uniforms = {
        {"uMVP",
         [this](std::shared_ptr<Shader> shader) {
             glm::mat4 model = glm::mat4(1.0f);
             glm::vec3 scale(0.3f);
             model = glm::scale(model, scale);
             float angle = t * glm::radians(90.0f);
             model *= glm::rotate(glm::mat4(1.0f), angle, {1.0f, 1.0f, 0.0f});
             glm::mat4 mvp = projMat * viewMat * model;
             shader->setUniformMat4f("uMVP", mvp);
         }},
    };
    mesh->setUniforms(uniforms);

    mesh->draw();
}
