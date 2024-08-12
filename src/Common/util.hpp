#pragma once

struct SimulationParams {
    int iter;
    float diffusion;
    float viscosity;
    float dt;
};

enum class ObjectType {
    None,
    Sphere,
    Cylinder,
    Plane,
    Cube,
};
