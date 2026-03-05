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

private:
    std::vector<Particle> particles;
    float gravity = -9.81f;
    float particleMass = 0.02f;
};
