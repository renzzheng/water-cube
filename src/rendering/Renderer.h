#pragma once
#include "Shader.h"
#include "Camera.h"
#include "../simulation/SPHSystem.h"
#include "../simulation/MarchingCubes.h"
#include "../simulation/MarchingCubes.h"

class Renderer {
public:
    Renderer();
    void draw(Camera& camera, int screenWidth, int screenHeight);
    void drawParticles(SPHSystem& sph, Camera& camera, int screenWidth, int screenHeight);
    void drawFluidSurface(MarchingCubes& mc, Camera& camera, int screenWidth, int screenHeight);

private:
    // cube
    unsigned int VAO, VBO, EBO;
    Shader shader;

    // particles
    unsigned int particleVAO, particleVBO;
    Shader particleShader;

    // fluid surface
    unsigned int fluidVAO, fluidVBO;
    Shader fluidShader;
};