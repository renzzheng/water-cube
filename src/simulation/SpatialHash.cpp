// neighbor search using spatial hashing
#include "SpatialHash.h"
#include <cmath>

SpatialHash::SpatialHash(float cellSize) : cellSize(cellSize) {}

glm::ivec3 SpatialHash::getCell(glm::vec3 position) {
    return glm::ivec3(
        (int)floor(position.x / cellSize),
        (int)floor(position.y / cellSize),
        (int)floor(position.z / cellSize)
    );
}

size_t SpatialHash::hash(int x, int y, int z) {
    return ((size_t)(x * 73856093)) ^ ((size_t)(y * 19349663)) ^ ((size_t)(z * 83492791));
}

void SpatialHash::build(std::vector<Particle>& particles) {
    table.clear();
    for (Particle& p : particles) {
        glm::ivec3 cell = getCell(p.position);
        size_t key = hash(cell.x, cell.y, cell.z);
        table[key].push_back(&p);
    }
}

std::vector<Particle*> SpatialHash::getNeighbors(glm::vec3 position) {
    std::vector<Particle*> neighbors;
    glm::ivec3 cell = getCell(position);

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                size_t key = hash(cell.x + x, cell.y + y, cell.z + z);
                auto it = table.find(key);
                if (it != table.end()) {
                    for (Particle* p : it->second) {
                        neighbors.push_back(p);
                    }
                }
            }
        }
    }
    return neighbors;
}