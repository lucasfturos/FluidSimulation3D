#pragma once

#include "../Common/util.hpp"
#include "../GLObjects/index_buffer.hpp"
#include "../GLObjects/shader.hpp"
#include "../GLObjects/vertex_array.hpp"
#include "../Objects/cylinder.hpp"
#include "../Objects/sphere.hpp"

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
    std::shared_ptr<VertexArray> va;
    std::shared_ptr<VertexBuffer> vb;
    std::shared_ptr<IndexBuffer> ib;
    std::shared_ptr<Shader> shader;

    std::vector<GLuint> indices;
    std::vector<glm::vec3> vertices;

    void update();

  public:
    Objects(glm::mat4, glm::mat4);

    void setObjectType(ObjectType);
    void setTime(float);
    void setup();
    void run();
};