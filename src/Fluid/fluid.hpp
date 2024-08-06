#pragma once

#include "../Common/perlin.hpp"
#include "../GLObjects/Texture/texture.hpp"
#include "../GLObjects/index_buffer.hpp"
#include "../GLObjects/shader.hpp"
#include "../GLObjects/vertex_array.hpp"

#include <algorithm>
#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Fluid {
  protected:
    struct SimulationParameters {
        int iter;
        int scale;
        float diffusion;
        float viscosity;
        float dt;
    };

    const int N = 270;
    const int nSize = N * N * N;

  private:
    std::shared_ptr<Perlin> perlin;
    std::vector<float> s;
    std::vector<float> density;
    std::vector<float> Vx, Vy, Vz;
    std::vector<float> Vx0, Vy0, Vz0;
    float t;

    std::shared_ptr<Shader> shader;
    glm::mat4 viewMat;
    glm::mat4 projMat;

  private:
    SimulationParameters params;

    std::shared_ptr<VertexArray> va;
    std::shared_ptr<VertexBuffer> vb;

    void loadParams(const std::string &);

    // Logic
    void step();
    void fadeDensity();
    void addDensity(glm::ivec3, float);
    void addVelocity(glm::ivec3, glm::vec3);
    void addTurbulence(glm::ivec3, float, glm::vec3);

    // Util
    int IX(int, int, int);
    void setBND(int, std::vector<float> &);
    void project(std::vector<float> &, std::vector<float> &,
                 std::vector<float> &, std::vector<float> &,
                 std::vector<float> &);
    void advect(int, std::vector<float> &, const std::vector<float> &,
                const std::vector<float> &, const std::vector<float> &,
                const std::vector<float> &, float);
    void linSolve(int, std::vector<float> &, const std::vector<float> &, float,
                  float);
    void diffuse(int, std::vector<float> &x, const std::vector<float> &, float,
                 float);

  public:
    Fluid(glm::mat4, glm::mat4);

    void setFilename(const std::string &);
    void setTime(float);
    void setup();
    void run();
};