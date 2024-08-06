#include "fluid.hpp"
#include "../Objects/cube.hpp"
#include <jsoncpp/json/json.h>

Fluid::Fluid(glm::mat4 view, glm::mat4 projection)
    : perlin(std::make_shared<Perlin>()), s(nSize, 0.0f), density(nSize, 0.0f),
      Vx(nSize, 0.0f), Vy(nSize, 0.0f), Vz(nSize, 0.0f), Vx0(nSize, 0.0f),
      Vy0(nSize, 0.0f), Vz0(nSize, 0.0f), t(0.0f), viewMat(view),
      projMat(projection) {}

void Fluid::setFilename(const std::string &filename) { loadParams(filename); }

void Fluid::setTime(float time) { t = time; }

void Fluid::loadParams(const std::string &filename) {
    std::ifstream file(filename, std::ifstream::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    Json::Value root;
    file >> root;
    params = {
        .iter = root.get("iter", 16).asInt(),
        .scale = root.get("scale", 8).asInt(),
        .diffusion = root.get("diffusion", 1.0e-5).asFloat(),
        .viscosity = root.get("viscosity", 1.0e-6).asFloat(),
        .dt = root.get("dt", 0.01).asFloat(),
    };
}

void Fluid::setup() {
    va = std::make_shared<VertexArray>();
    vb = std::make_shared<VertexBuffer>(cubeVertices);

    VertexBufferLayout layout;
    layout.push<GLfloat>(3);

    va->addBuffer(*vb, layout);

    shader = std::make_shared<Shader>("assets/shader/fluid.shader");
}

void Fluid::run() {
    shader->bind();
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 translation(-8.0f, 0.0f, 0.0f);
    model = glm::translate(model, translation);

    float scaleFactor = 2.0f;
    glm::vec3 scale(scaleFactor);
    model = glm::scale(model, scale);

    float angle = t * glm::radians(90.0f);
    glm::mat4 rotationMatrix =
        glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));

    glm::mat4 mvp = projMat * viewMat * model * rotationMatrix;

    shader->setUniformMat4f("uMVP", mvp);

    va->bind();
    vb->bind();

    glDrawArrays(GL_TRIANGLES, 0, cubeVertices.size());

    va->unbind();
    vb->unbind();
    shader->unbind();
}
