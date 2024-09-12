#pragma once

#include "index_buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"

#include <functional>
#include <memory>

struct CustomVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

template <typename VertexType> class Mesh {
  public:
    Mesh(const std::vector<VertexType> &vertices,
         const std::vector<GLuint> &indices,
         const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath)
        : va(std::make_shared<VertexArray>()),
          vb(std::make_shared<VertexBuffer<VertexType>>(vertices)),
          ib(std::make_shared<IndexBuffer>(indices)),
          shader(
              std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath)),
          hasTexture(false) {}

    using UniformsMap =
        std::unordered_map<std::string,
                           std::function<void(std::shared_ptr<Shader>)>>;

    template <typename LayoutType>
    void setup(const std::vector<int> &attributeSizes) {
        VertexBufferLayout layout;
        for (int size : attributeSizes) {
            layout.push<LayoutType>(size);
        }
        va->addBuffer(*vb, layout);
    }

    void setUniforms(const UniformsMap &uniforms) {
        shader->bind();
        for (const auto &[name, setUniform] : uniforms) {
            setUniform(shader);
        }
        shader->unbind();
    }

    void draw(unsigned int slot = 0) {
        shader->bind();
        va->bind();
        vb->bind();
        ib->bind();

        if (hasTexture && texture) {
            texture->bind(slot);
        }

        glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr);

        va->unbind();
        vb->unbind();
        ib->unbind();
        shader->unbind();
        if (hasTexture) {
            texture->unbind();
        }
    }

    void updateTexture(const std::vector<float> &data, int width, int height,
                       int depth) {
        if (texture) {
            texture->updateData(data, width, height, depth, GL_RGB, GL_FLOAT);
        }
    }

    void setTexture(std::shared_ptr<Texture> newTexture) {
        texture = newTexture;
        hasTexture = (texture != nullptr);
    }

  private:
    std::shared_ptr<VertexArray> va;
    std::shared_ptr<VertexBuffer<VertexType>> vb;
    std::shared_ptr<IndexBuffer> ib;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;
    bool hasTexture;
};
