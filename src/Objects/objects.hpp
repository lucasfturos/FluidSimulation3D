#pragma once

#include "Common/util.hpp"
#include "GLObjects/mesh.hpp"
#include "Objects/cylinder.hpp"
#include "Objects/sphere.hpp"

#include <memory>

class Objects {
  protected:
    const glm::mat4 m_ViewMatrix =
        glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

  private:
    std::shared_ptr<Cylinder> cylinder;
    std::shared_ptr<Sphere> sphere;
    ObjectType objectType;

    glm::mat4 projMat;
    glm::vec3 position;

    float m_Time;

  private:
    std::shared_ptr<Mesh<glm::vec3>> mesh;

    std::vector<GLuint> indices;
    std::vector<glm::vec3> vertices;

    void update();

  public:
    Objects(glm::mat4);

    void setObjectType(ObjectType type) {
        if (objectType != type) {
            objectType = type;
            update();
        }
    }
    void setPosition(const glm::vec3 &newPosition) { position = newPosition; }
    void setTime(float time) { m_Time = time; }

    glm::vec3 getPosition() const { return position; }
    ObjectType getObjectType() const { return objectType; }
    std::shared_ptr<Sphere> getSphere() const { return sphere; }
    std::shared_ptr<Cylinder> getCylinder() const { return cylinder; }

    void setup();
    void run();
};