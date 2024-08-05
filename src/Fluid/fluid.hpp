#pragma once

#include "../Common/perlin.hpp"
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
    const int nSize = N * N;

  private:
    std::shared_ptr<Perlin> perlin;
    std::vector<float> s;
    std::vector<float> density;
    std::vector<float> Vx, Vy, Vz;
    std::vector<float> Vx0, Vy0, Vz0;

  private:
    SimulationParameters params;

    void loadParams(const std::string &);

    // Draw
    void draw();

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
    Fluid();

    void setFilename(const std::string &);
    void run();
};