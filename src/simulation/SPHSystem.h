#pragma once
#include <glm/glm.hpp>
#include <vector>

// spawn water particles in a grid inside the cube
struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float     mass;
    float     density;
    float     pressure;
};

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

private:
    std::vector<Particle> particles;
    float gravity     = -9.81f;
    float particleMass = 0.02f;
    float h           = 0.8f;   
    float restDensity = 15.0f;  
    float stiffness   = 5.0f;   
    float viscosity   = 0.1f;
    float maxVel = 5.0f;
};
