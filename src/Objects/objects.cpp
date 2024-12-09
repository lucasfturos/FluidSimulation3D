#include "objects.hpp"
#include "Objects/cube.hpp"
#include "Objects/plane.hpp"

Objects::Objects(glm::mat4 projection)
    : cylinder(std::make_shared<Cylinder>(1, 0.3, 0.3, 20)),
      sphere(std::make_shared<Sphere>(1, 20)), objectType(ObjectType::Sphere),
      projMat(projection), position(1.0f, 0.0f, 0.0f), m_Time(0.0f) {}

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

    mesh = std::make_shared<Mesh<glm::vec3>>(
        vertices, indices, "assets/shader/Basic/vertex.shader",
        "assets/shader/Basic/fragment.shader");
    mesh->setup<GLfloat>({3});
}

void Objects::setup() { update(); }

void Objects::run() {
    if (objectType == ObjectType::None) {
        return;
    }

    Mesh<glm::vec3>::UniformsMap uniforms = {
        {"uMVP",
         [this](std::shared_ptr<Shader> shader) {
             glm::mat4 model = glm::mat4(1.0f);
             model = glm::translate(model, position);

             glm::vec3 scale(0.2f);
             model = glm::scale(model, scale);

             float angle = m_Time * glm::radians(90.0f);
             glm::mat4 rotationMatrix = glm::rotate(
                 glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));
             model *= rotationMatrix;

             glm::mat4 mvp = projMat * m_ViewMatrix * model;
             shader->setUniformMat4f("uMVP", mvp);
         }},
    };

    mesh->setUniforms(uniforms);
    mesh->draw();
}
