#pragma once

#include "Common/util.hpp"
#include "GLObjects/mesh.hpp"
#include "Objects/cylinder.hpp"
#include "Objects/sphere.hpp"

#include <memory>

class Objects {
  private:
    std::shared_ptr<Cylinder> cylinder;
    std::shared_ptr<Sphere> sphere;
    ObjectType objectType;

    glm::mat4 viewMat;
    glm::mat4 projMat;
    float t;

  private:
    std::shared_ptr<Mesh<glm::vec3>> mesh;

    std::vector<GLuint> indices;
    std::vector<glm::vec3> vertices;

    void update();

  public:
    Objects(glm::mat4, glm::mat4);

    void setObjectType(ObjectType type) {
        if (objectType != type) {
            objectType = type;
            update();
        }
    }

    void setTime(float time) { t = time; }

    void setup();
    void run();
};