#pragma once

#include <GL/glew.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <unordered_map>
#include <vector>

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader {
  private:
    GLuint m_bufferID;
    std::unordered_map<std::string, GLint> m_uniformLocationCache;

  public:
    Shader(const std::string &filepath) : m_bufferID(0) {
        ShaderProgramSource source = parseShader(filepath);
        m_bufferID = createShader(source.vertexSource, source.fragmentSource);
    }

    ~Shader() { glDeleteProgram(m_bufferID); }

    void bind() const { glUseProgram(m_bufferID); }
    void unbind() const { glUseProgram(0); }

    void setUniform3f(const std::string &name, glm::vec3 value) {
        GLint location = getUniformLocation(name);
        glUniform3f(location, value.x, value.y, value.z);
    }

    void setUniform3fv(const std::string &name,
                       const std::vector<glm::vec3> &values) {
        GLint location = getUniformLocation(name);
        glUniform3fv(location, static_cast<GLsizei>(values.size()),
                     glm::value_ptr(values[0]));
    }

    void setUniform4f(const std::string &name, glm::vec4 value) {
        GLint location = getUniformLocation(name);
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void setUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
        GLint location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    }

  private:
    GLuint createShader(const std::string &vertex_shader,
                        const std::string &fragment_shader) {
        GLuint program = glCreateProgram();
        GLuint vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    ShaderProgramSource parseShader(const std::string &filepath) {
        enum class ShaderType {
            NONE = -1,
            VERTEX = 0,
            FRAGMENT = 1,
        };

        std::string line;
        std::ifstream stream(filepath);
        std::vector<std::stringstream> ss(2);
        auto type = ShaderType::NONE;
        while (std::getline(stream, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos) {
                    type = ShaderType::VERTEX;
                } else if (line.find("fragment") != std::string::npos) {
                    type = ShaderType::FRAGMENT;
                }
            } else {
                ss[static_cast<int>(type)] << line << '\n';
            }
        }

        return {.vertexSource = ss[0].str(), .fragmentSource = ss[1].str()};
    }

    GLuint compileShader(GLuint type, const std::string &source) {
        GLuint id = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int sucess;
        glGetShaderiv(id, GL_COMPILE_STATUS, &sucess);
        if (!sucess) {
            int len;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
            char *message = (char *)alloca(len * sizeof(char));
            glGetShaderInfoLog(id, len, &len, message);
            throw std::runtime_error(
                "Failed compile " +
                std::string(
                    (type == GL_VERTEX_SHADER ? "vertex" : "fragment")) +
                " shader!\n" + message + '\n');
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    GLint getUniformLocation(const std::string &name) {
        if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
            return m_uniformLocationCache[name];
        }

        GLint location = glGetUniformLocation(m_bufferID, name.c_str());
        if (location == -1) {
            throw std::runtime_error("Warning: Uniform '" + name +
                                     "' doesn't exit!\n");
        }

        m_uniformLocationCache[name] = location;
        return location;
    }
};