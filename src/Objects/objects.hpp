#pragma once

#include "../GLObjects/index_buffer.hpp"
#include "../GLObjects/shader.hpp"
#include "../GLObjects/vertex_array.hpp"

#include <memory>

class Objects {

  private:
    glm::mat4 viewMat;
    glm::mat4 projMat;
    float t;

    std::shared_ptr<VertexArray> va;
    std::shared_ptr<VertexBuffer> vb;
    std::shared_ptr<IndexBuffer> ib;
    std::shared_ptr<Shader> shader;

    std::vector<GLuint> indices;

  public:
    Objects(glm::mat4);

    void setTime(float);
    void setup();
    void run();
};