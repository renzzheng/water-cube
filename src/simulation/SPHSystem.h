#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Particle.h"
#include "SpatialHash.h"

// // spawn water particles in a grid inside the cube
// struct Particle {
//     glm::vec3 position;
//     glm::vec3 velocity;
//     glm::vec3 acceleration;
//     float     mass;
//     float     density;
//     float     pressure;
// };

void setGravityDirection(glm::vec3 dir);

class SPHSystem {
public:
    SPHSystem();
    void init();
    void update(float dt); // move particles forward one timestep
    std::vector<Particle> getParticleData();

    void computeDensityPressure();  // Question 1 and 2
    void computeForces();           // Question 3
    glm::vec3 gradW(glm::vec3 r);   // smoothing kernel gradient
    float W(float r);               // smoothing kernel

    void setGravityDirection(glm::vec3 dir);

private:
    std::vector<Particle> particles;
    SpatialHash spatialHash;
    float gravity     = -9.81f;
    float particleMass = 0.02f;
    float h           = 0.35f;
    float restDensity = 15.0f;
    float stiffness   = 5.0f;
    float viscosity   = 0.1f;
    float maxVel = 5.0f;
    glm::vec3 gravityDir = glm::vec3(0.0f, -1.0f, 0.0f);
};
