#include "fluid.hpp"
#include <jsoncpp/json/json.h>

Fluid::Fluid()
    : perlin(std::make_shared<Perlin>()), s(nSize, 0.0f), density(nSize, 0.0f),
      Vx(nSize, 0.0f), Vy(nSize, 0.0f), Vz(nSize, 0.0f), Vx0(nSize, 0.0f),
      Vy0(nSize, 0.0f), Vz0(nSize, 0.0f) {}

void Fluid::setFilename(const std::string &filename) { loadParams(filename); }

void Fluid::loadParams(const std::string &filename) {
    std::ifstream file(filename, std::ifstream::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    Json::Value root;
    file >> root;
    params = {
        .iter = root.get("iter", 16).asInt(),
        .scale = root.get("scale", 8).asInt(),
        .diffusion = root.get("diffusion", 1.0e-5).asFloat(),
        .viscosity = root.get("viscosity", 1.0e-6).asFloat(),
        .dt = root.get("dt", 0.01).asFloat(),
    };
}

void Fluid::run() {}
