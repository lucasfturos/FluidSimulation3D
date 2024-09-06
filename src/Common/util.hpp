#pragma once

struct SimulationParams {
    int iter;
    float diffusion;
    float viscosity;
    float dt;
    float fadeRate;
};

enum class ObjectType {
    None,
    Sphere,
    Cylinder,
    Plane,
    Cube,
};
