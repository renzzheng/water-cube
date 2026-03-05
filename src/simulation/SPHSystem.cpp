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
    computeDensityPressure();
    computeForces();

    // apply gravity
    for (Particle& p : particles) {
        // p.acceleration = glm::vec3(0.0f, gravity, 0.0f); // reset each frame

    // update velocity from acceleration and position from velocity
    p.velocity += p.acceleration * dt;
    // cap velocity to prevent explosion
    float maxVel = 2.0f;
    if (glm::length(p.velocity) > maxVel)
        p.velocity = glm::normalize(p.velocity) * maxVel;

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

float SPHSystem::W(float r) {
    if (r > h) return 0.0f; // outside smoothing radius, no influence
    float coeff = 315.0f / (64.0f * M_PI * pow(h, 9));
    return coeff * (float)pow(h * h - r * r, 3);
}

glm::vec3 SPHSystem::gradW(glm::vec3 r) {
    float len = glm::length(r); // distance between particles
    if (len > h || len < 0.0001f) return glm::vec3(0.0f); // outside smoothing radius or too close, no influence
    
    float coeff = -945.0f / (32.0f * M_PI * pow(h, 9));
    float kernel = (float)pow(h * h - len * len, 2);
    return coeff * kernel * r;
}

void SPHSystem::computeDensityPressure() {
    for(Particle& p : particles) {
        p.density = 0.0f;
        for(Particle& neighbor : particles) {
            float r = glm::length(p.position - neighbor.position);
            p.density += neighbor.mass * W(r);
        }
        p.pressure = stiffness * (p.density - restDensity);
    }
}

void SPHSystem::computeForces() {
    for (Particle& p : particles) {
        glm::vec3 pressureForce(0.0f);
        glm::vec3 viscosityForce(0.0f);

        for (Particle& neighbor : particles) {
            if (&p == &neighbor) continue; // skip self

            glm::vec3 r = p.position - neighbor.position;
            float len = glm::length(r);
            if (len > h) continue; // skip if outside radius

            // pressure force
            // your turn -- use gradW, and average pressure of p and neighbor
            // formula: -mass * (p.pressure + neighbor.pressure) / (2 * neighbor.density) * gradW(r)
            glm::vec3 pressureGrad = -neighbor.mass * (p.pressure + neighbor.pressure) / (2.0f * neighbor.density) * gradW(r);
            pressureForce += pressureGrad;

            // viscosity force
            // your turn -- formula: viscosity * mass * (neighbor.velocity - p.velocity) / neighbor.density * W(len)
            glm::vec3 visc = viscosity * neighbor.mass * (neighbor.velocity - p.velocity) / neighbor.density * W(len);
            viscosityForce += visc;
        }

        // set acceleration from forces + gravity
        p.acceleration = (pressureForce + viscosityForce) / p.density;
        p.acceleration.y += gravity;
    }
}