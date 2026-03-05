#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "SPHSystem.h"

class MarchingCubes {
public:
    MarchingCubes(int gridSize, float cubeSize);

    void update(const std::vector<Particle>& particles);
    std::vector<glm::vec3>& getVertices();

private:
    int gridSize;
    float cubeSize;
    float isoLevel = 0.5f;

    std::vector<std::vector<std::vector<float>>> grid;
    std::vector<glm::vec3> vertices;

    void buildGrid(const std::vector<Particle>& particles);
    void march();
    glm::vec3 interpolate(glm::vec3 p1, glm::vec3 p2, float v1, float v2);
    glm::vec3 gridPos(int x, int y, int z);
};