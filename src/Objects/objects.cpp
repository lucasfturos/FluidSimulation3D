#include "objects.hpp"
#include "../Objects/cube.hpp"
#include "../Objects/plane.hpp"

Objects::Objects(glm::mat4 view, glm::mat4 projection)
    : cylinder(std::make_shared<Cylinder>(8, 2, 2, 20)),
      sphere(std::make_shared<Sphere>(5, 20)), objectType(ObjectType::None),
      viewMat(view), projMat(projection), t(0.0f) {}

void Objects::setObjectType(ObjectType type) {
    if (objectType != type) {
        objectType = type;
        update();
    }
}

void Objects::setTime(float time) { t = time; }

void Objects::update() {
    vertices.clear();
    indices.clear();

    switch (objectType) {
    case ObjectType::None:
        return;
        break;
    case ObjectType::Sphere:
        vertices = sphere->genVertices();
        indices = sphere->genIndices();
        break;
    case ObjectType::Cylinder:
        vertices = cylinder->genVertices();
        indices = cylinder->genIndices();
        break;
    case ObjectType::Plane:
        vertices = planeVertices;
        indices = planeIndices;
        break;
    case ObjectType::Cube:
        vertices = cubeVertices;
        indices = cubeIndices;
        break;
    }

    va = std::make_shared<VertexArray>();
    vb = std::make_shared<VertexBuffer>(vertices);
    ib = std::make_shared<IndexBuffer>(indices);

    VertexBufferLayout layout;
    layout.push<GLfloat>(3);

    va->addBuffer(*vb, layout);

    shader = std::make_shared<Shader>("assets/shader/basic.shader");
}

void Objects::setup() { update(); }

void Objects::run() {
    if (objectType == ObjectType::None) {
        return;
    }

    shader->bind();

    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 translation(8.0f, 0.0f, 0.0f);
    model = glm::translate(model, translation);

    float scaleFactor = 1.0f;
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

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    va->unbind();
    vb->unbind();
    ib->unbind();
    shader->unbind();
}
