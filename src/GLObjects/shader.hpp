#pragma once

#include <GL/glew.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
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
    Shader(const std::string &vertexPath, const std::string &fragmentPath)
        : m_bufferID(0) {
        std::string vertexSource = loadShader(vertexPath);
        std::string fragmentSource = loadShader(fragmentPath);
        m_bufferID = createShader(vertexSource, fragmentSource);
    }

    ~Shader() { glDeleteProgram(m_bufferID); }

    void bind() const { glUseProgram(m_bufferID); }
    void unbind() const { glUseProgram(0); }

    void setUniform1i(const std::string &name, GLint value) {
        GLint location = getUniformLocation(name);
        glUniform1i(location, value);
    }

    void setUniform1f(const std::string &name, GLfloat value) {
        GLint location = getUniformLocation(name);
        glUniform1f(location, value);
    }

    void setUniform2f(const std::string &name, glm::vec2 value) {
        GLint location = getUniformLocation(name);
        glUniform2f(location, value.x, value.y);
    }

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

    void setUniformMat3f(const std::string &name, const glm::mat3 &matrix) {
        GLint location = getUniformLocation(name);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void setUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
        GLint location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

  private:
    std::string loadShader(const std::string &filepath) {
        std::ifstream file(filepath);
        std::stringstream stream;
        stream << file.rdbuf();
        return stream.str();
    }

    GLuint createShader(const std::string &vertexSource,
                        const std::string &fragmentSource) {
        GLuint program = glCreateProgram();
        GLuint vs = compileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    GLuint compileShader(GLuint type, const std::string &source) {
        GLuint id = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char *message = (char *)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            throw std::runtime_error("Failed compile shader!\n" +
                                     std::string(message) + '\n');
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
            std::cerr << "Warning: Uniform '" << name << "' doesn't exist!\n";
        }

        m_uniformLocationCache[name] = location;
        return location;
    }
};
