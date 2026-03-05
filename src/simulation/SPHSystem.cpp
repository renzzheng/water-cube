#include <iostream>
#include "SPHSystem.h"

SPHSystem::SPHSystem() {
    init();
}

void SPHSystem::init() {
    particles.clear();
    float spacing = 0.2f;

    for (float x = -0.8f; x < 0.8f; x += spacing) {
        for (float y = -0.8f; y < 0.0f; y += spacing) {
            for (float z = -0.8f; z < 0.8f; z += spacing) {

                Particle p;
                p.position     = glm::vec3(x, y, z);
                p.velocity     = glm::vec3(0.0f, 0.0f, 0.0f);
                p.acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
                p.mass         = particleMass;
                p.density      = 0.0f;
                p.pressure     = 0.0f;

                particles.push_back(p);
            }
        }
    }

    std::cout << "Particle count: " << particles.size() << std::endl;
}

void SPHSystem::update(float dt) {
    // apply gravity
    for (Particle& p : particles) {
        p.acceleration = glm::vec3(0.0f, gravity, 0.0f); // reset each frame

    // update velocity from acceleration and position from velocity
    p.velocity += p.acceleration * dt;
    p.position += p.velocity * dt;

    // wall collisions for all 6 walls
    float damping = -0.5f;
    float wall = 1.0f;

    if (p.position.y < -wall) { p.position.y = -wall; p.velocity.y *= damping; }
    if (p.position.y >  wall) { p.position.y =  wall; p.velocity.y *= damping; }
    if (p.position.x < -wall) { p.position.x = -wall; p.velocity.x *= damping; }
    if (p.position.x >  wall) { p.position.x =  wall; p.velocity.x *= damping; }
    if (p.position.z < -wall) { p.position.z = -wall; p.velocity.z *= damping; }
    if (p.position.z >  wall) { p.position.z =  wall; p.velocity.z *= damping; }
    }
}

std::vector<Particle> SPHSystem::getParticleData() {
    return particles;
}