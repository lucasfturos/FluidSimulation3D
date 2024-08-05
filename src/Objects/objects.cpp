#include "objects.hpp"
#include "../Objects/cylinder.hpp"

Objects::Objects(glm::mat4 view)
    : viewMat(view),
      projMat(glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f)),
      t(0.0f) {}

void Objects::setTime(float time) { t = time; }

void Objects::setup() {
    auto cylinder = std::make_shared<Cylinder>(8, 5, 5, 20);
    std::vector<glm::vec3> vertices = cylinder->genVertices();
    indices = cylinder->genIndices();

    va = std::make_shared<VertexArray>();
    vb = std::make_shared<VertexBuffer>(vertices);
    ib = std::make_shared<IndexBuffer>(indices);

    VertexBufferLayout layout;
    layout.push<GLfloat>(3);

    va->addBuffer(*vb, layout);

    shader = std::make_shared<Shader>("assets/shader/basic.shader");
}

void Objects::run() {
    shader->bind();
    glm::mat4 mvp = projMat * viewMat;

    float angle = t * glm::radians(90.0f);
    glm::mat4 rotationMatrix =
        glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));

    mvp *= rotationMatrix;
    shader->setUniformMat4f("uMVP", mvp);

    va->bind();
    ib->bind();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    ib->unbind();
    va->unbind();
    shader->unbind();
}