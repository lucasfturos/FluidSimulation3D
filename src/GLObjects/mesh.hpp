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
         const std::vector<GLuint> &faces, const std::string &vertexShaderPath,
         const std::string &fragmentShaderPath)
        : m_VertexArray(std::make_shared<VertexArray>()),
          m_VerticesBuffer(
              std::make_shared<VertexBuffer<VertexType>>(vertices)),
          m_FacesBuffer(std::make_shared<IndexBuffer>(faces)),
          m_Shader(
              std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath)),
          m_HasTexture(false) {}

    using UniformsMap =
        std::unordered_map<std::string,
                           std::function<void(std::shared_ptr<Shader>)>>;

    template <typename LayoutType>
    void setup(const std::vector<int> &attributeSizes) {
        VertexBufferLayout layout;
        for (int size : attributeSizes) {
            layout.push<LayoutType>(size);
        }
        m_VertexArray->addBuffer(*m_VerticesBuffer, layout);
    }

    void setUniforms(const UniformsMap &uniforms) {
        m_Shader->bind();
        for (const auto &[name, setUniform] : uniforms) {
            setUniform(m_Shader);
        }
        m_Shader->unbind();
    }

    void draw(unsigned int slot = 0) {
        m_Shader->bind();
        m_VertexArray->bind();
        m_VerticesBuffer->bind();
        m_FacesBuffer->bind();

        if (m_HasTexture && m_Texture) {
            m_Texture->bind(slot);
        }

        glDrawElements(GL_TRIANGLES, m_FacesBuffer->getCount(), GL_UNSIGNED_INT,
                       nullptr);

        m_VertexArray->unbind();
        m_VerticesBuffer->unbind();
        m_FacesBuffer->unbind();
        m_Shader->unbind();
        if (m_HasTexture) {
            m_Texture->unbind();
        }
    }

    void updateTexture(const std::vector<float> &data, int width, int height,
                       int depth) {
        if (m_Texture) {
            m_Texture->updateData(data, width, height, depth, GL_RGB, GL_FLOAT);
        }
    }

    void setTexture(std::shared_ptr<Texture> newTexture) {
        m_Texture = newTexture;
        m_HasTexture = (m_Texture != nullptr);
    }

  private:
    std::shared_ptr<VertexArray> m_VertexArray;
    std::shared_ptr<VertexBuffer<VertexType>> m_VerticesBuffer;
    std::shared_ptr<IndexBuffer> m_FacesBuffer;
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Texture> m_Texture;
    bool m_HasTexture;
};
