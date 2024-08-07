#include "fluid.hpp"
#include "../Common/color.hpp"
#include "../Objects/cube.hpp"
#include <jsoncpp/json/json.h>

Fluid::Fluid(glm::mat4 view, glm::mat4 projection)
    : perlin(std::make_shared<Perlin>()), s(nSize, 0.0f), density(nSize, 0.0f),
      Vx(nSize, 0.0f), Vy(nSize, 0.0f), Vz(nSize, 0.0f), Vx0(nSize, 0.0f),
      Vy0(nSize, 0.0f), Vz0(nSize, 0.0f), colors(nSize, 0.0f), t(0.0f),
      viewMat(view), projMat(projection) {}

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

void Fluid::drawDensity() {
    int scale = params.scale;
    colors.clear();
    for (const auto &vertex : cubeVertices) {
        int x = static_cast<int>(vertex.x) * scale;
        int y = static_cast<int>(vertex.y) * scale;
        int z = static_cast<int>(vertex.z) * scale;

        int index = IX(x, y, z);
        float d = density[index];
        glm::vec3 color = getColorByValue(std::fmod((d + 100), 255.0f),
                                          200 / 255.0f, 200 / 255.0f);
        colors.push_back(color.r);
        colors.push_back(color.g);
        colors.push_back(color.b);
    }

    texture->updateData(colors, N, N, N, GL_RGB, GL_FLOAT);
}

void Fluid::setup() {
    va = std::make_shared<VertexArray>();
    vb = std::make_shared<VertexBuffer>(cubeVertices);
    ib = std::make_shared<IndexBuffer>(cubeIndices);

    VertexBufferLayout layout;
    layout.push<GLfloat>(3);

    va->addBuffer(*vb, layout);

    shader = std::make_shared<Shader>("assets/shader/fluid.shader");

    texture = std::make_shared<Texture>(N, N, N, GL_RGB, GL_FLOAT);
    shader->bind();
    shader->setUniform1i("densityTex", 0);
}

void Fluid::run() {
    static float time = 0.0f;
    step();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(50.0f, 350.0f);

    for (const auto &vertex : cubeVertices) {
        for (int i = 0; i < 6; ++i) {
            addDensity(vertex, dis(gen));
        }

        for (int i = 0; i < 6; ++i) {
            float angle = 2 * M_PI;
            float vX = std::cos(angle) * 0.1f;
            float vY = std::sin(angle) * 0.1f;
            addVelocity(vertex, glm::vec3(vX, vY, std::atan(vY / vX)));
            addTurbulence(vertex, time, glm::vec3(vX, vY, std::atan(vY / vX)));
            time += 0.01;
        }
    }

    drawDensity();

    fadeDensity();

    shader->bind();

    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 translation(-8.0f, 0.0f, 0.0f);
    model = glm::translate(model, translation);

    float scaleFactor = 3.0f;
    glm::vec3 scale(scaleFactor);
    model = glm::scale(model, scale);

    float angle = t * glm::radians(90.0f);
    glm::mat4 rotationMatrix =
        glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));

    glm::mat4 mvp = projMat * viewMat * model * rotationMatrix;

    shader->setUniformMat4f("uMVP", mvp);

    va->bind();
    vb->bind();
    ib->bind();
    texture->bind();
    
    glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, nullptr);

    va->unbind();
    vb->unbind();
    ib->unbind();
    shader->unbind();
    texture->unbind();
}
