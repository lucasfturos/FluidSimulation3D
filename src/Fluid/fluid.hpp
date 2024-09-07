#pragma once

#include "Common/perlin.hpp"
#include "Common/util.hpp"
#include "GLObjects/index_buffer.hpp"
#include "GLObjects/shader.hpp"
#include "GLObjects/texture.hpp"
#include "GLObjects/vertex_array.hpp"
#include "Objects/cube.hpp"

#include <algorithm>
#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Fluid {
  protected:
    const int N = 25;
    const int nSize = N * N * N;

  private:
    std::shared_ptr<Perlin> perlin;

    std::vector<float> s;
    std::vector<float> density;
    std::vector<float> Vx, Vy, Vz;
    std::vector<float> Vx0, Vy0, Vz0;
    std::vector<float> colors;

    glm::mat4 viewMat;
    glm::mat4 projMat;
    glm::vec3 gravity;

    float t;

  private:
    SimulationParams params;

    std::shared_ptr<VertexArray> va;
    std::shared_ptr<VertexBuffer<glm::vec3>> vb;
    std::shared_ptr<IndexBuffer> ib;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;

    void drawDensity();
    void draw();

    // Logic
    void step();
    void fadeDensity();
    void setupFluidDynamics();
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
    Fluid(glm::mat4);

    void setTime(float);
    void setSimulationParams(SimulationParams);
    void setup();
    void run();
};