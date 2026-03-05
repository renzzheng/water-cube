#pragma once
#include "Shader.h"
#include "Camera.h"
#include "simulation/SPHSystem.h"

class Renderer {
public:
    Renderer();
    void draw(Camera& camera, int screenWidth, int screenHeight);
    void drawParticles(SPHSystem& sph, Camera& camera, int screenWidth, int screenHeight);

private:
    // cube
    unsigned int VAO, VBO, EBO;
    Shader shader;

    // particles
    unsigned int particleVAO, particleVBO;
    Shader particleShader;
};