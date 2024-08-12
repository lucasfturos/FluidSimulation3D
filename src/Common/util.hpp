#pragma once

struct SimulationParams {
    int iter;
    float diffusion;
    float viscosity;
    float dt;
};

enum class ObjectType {
    Sphere,
    Cylinder,
    Plane,
    Cube,
};
