#pragma once
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Particle.h"

class SpatialHash {
public:
    SpatialHash(float cellSize);

    void build(std::vector<Particle>& particles);
    std::vector<Particle*> getNeighbors(glm::vec3 position);

private:
    float cellSize;
    std::unordered_map<size_t, std::vector<Particle*>> table;

    size_t hash(int x, int y, int z);
    glm::ivec3 getCell(glm::vec3 position);
};