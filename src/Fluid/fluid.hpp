#pragma once

#include "Common/perlin.hpp"
#include "Common/util.hpp"
#include "GLObjects/mesh.hpp"
#include "Objects/cube.hpp"

#include <algorithm>
#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Fluid {
  protected:
    const int N = 25;
    const int m_nSize = N * N * N;
    const glm::mat4 m_ViewMatrix =
        glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

  private:
    std::shared_ptr<Perlin> m_PerlinNoise;

    std::vector<float> m_TempDensity;
    std::vector<float> m_Density;
    std::vector<float> m_Vx, m_Vy, m_Vz;
    std::vector<float> m_Vx0, m_Vy0, m_Vz0;
    std::vector<float> m_Colors;

    glm::mat4 m_ProjMattrix;
    glm::mat4 m_ModelMatrix;
    glm::vec3 m_Gravity;

    float m_Time;

  private:
    std::shared_ptr<Mesh<glm::vec3>> m_Mesh;
    SimulationParams m_SimulParams;

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

    void setTime(float time) { m_Time = time; }
    void setSimulationParams(SimulationParams params) {
        m_SimulParams = params;
    }

    void setup();
    void run();
};