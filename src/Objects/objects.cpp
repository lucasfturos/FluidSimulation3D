#include "objects.hpp"
#include "../Objects/cube.hpp"
#include "../Objects/plane.hpp"

Objects::Objects(glm::mat4 view, glm::mat4 projection)
    : cylinder(std::make_shared<Cylinder>(8, 2, 2, 20)),
      sphere(std::make_shared<Sphere>(5, 20)), objectType(ObjectType::None),
      viewMat(view), projMat(projection), t(0.0f) {}

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

    mesh = std::make_shared<Mesh>(vertices, indices,
                                  "assets/shader/Basic/vertex.shader",
                                  "assets/shader/Basic/fragment.shader");
    mesh->setup<GLfloat>({3});
}

void Objects::setup() { update(); }

void Objects::run() {
    if (objectType == ObjectType::None) {
        return;
    }

    Mesh::UniformsMap uniforms = {
        {"uMVP",
         [this](std::shared_ptr<Shader> shader) {
             glm::mat4 model = glm::mat4(1.0f);
             glm::vec3 translation(8.0f, 0.0f, 0.0f);
             model = glm::translate(model, translation);

             float scaleFactor = 1.0f;
             glm::vec3 scale(scaleFactor);
             model = glm::scale(model, scale);

             float angle = t * glm::radians(90.0f);
             glm::mat4 rotationMatrix = glm::rotate(
                 glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));
             model *= rotationMatrix;

             glm::mat4 mvp = projMat * viewMat * model;
             shader->setUniformMat4f("uMVP", mvp);
         }},
    };

    mesh->setUniforms(uniforms);
    mesh->draw();
}
