#pragma once

struct SimulationParams {
    int iter;
    float diffusion;
    float viscosity;
    float dt;
    float fadeRate;
    bool activeGravity;
    float gravityScale;
};

enum class ObjectType {
    None,
    Sphere,
    Cylinder,
    Plane,
    Cube,
};
