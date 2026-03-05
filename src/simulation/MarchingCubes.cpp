#include "MarchingCubes.h"
#include "MarchingTables.h"
#include <cmath>

MarchingCubes::MarchingCubes(int gridSize, float cubeSize)
    : gridSize(gridSize), cubeSize(cubeSize) {
    // initialize 3D grid with zeros
    grid.resize(gridSize + 1,
        std::vector<std::vector<float>>(gridSize + 1,
            std::vector<float>(gridSize + 1, 0.0f)));
}

glm::vec3 MarchingCubes::gridPos(int x, int y, int z) {
    // convert grid index to world position
    float step = cubeSize / gridSize;
    return glm::vec3(
        -cubeSize/2 + x * step,
        -cubeSize/2 + y * step,
        -cubeSize/2 + z * step
    );
}

void MarchingCubes::buildGrid(const std::vector<Particle>& particles) {
    // clear grid
    for (auto& plane : grid)
        for (auto& row : plane)
            for (auto& cell : row)
                cell = 0.0f;

    float step = cubeSize / gridSize;

    // for each grid point, sum particle contributions
    for (int x = 0; x <= gridSize; x++) {
        for (int y = 0; y <= gridSize; y++) {
            for (int z = 0; z <= gridSize; z++) {
                glm::vec3 pos = gridPos(x, y, z);
                float density = 0.0f;

                for (const Particle& p : particles) {
                    float dist = glm::length(pos - p.position);
                    if (dist < 0.8f) {
                        float q = 1.0f - (dist / 0.8f);
                        density += q * q * q; // simple cubic kernel
                    }
                }

                // to make the surface smoother near the walls, we can add a contribution that increases density as we get closer to the walls
                // to simiulate the effect of surface tension pulling the fluid away from the walls and creating a more rounded surface
                // the meniscus effect is more pronounced near the surface, so we can apply this wall contribution only to grid points that are near the surface (e.g. y between -0.5 and 0.2)
                if (pos.y > -1.0f && pos.y < 0.2f) { // entire fluid height, not just surface
                    float wallDist = 0.3f;
                    if (pos.x < -1.0f + wallDist) density += (wallDist - (pos.x + 1.0f)) * 0.5f;
                    if (pos.x >  1.0f - wallDist) density += (wallDist - (1.0f - pos.x)) * 0.5f;
                    if (pos.z < -1.0f + wallDist) density += (wallDist - (pos.z + 1.0f)) * 0.5f;
                    if (pos.z >  1.0f - wallDist) density += (wallDist - (1.0f - pos.z)) * 0.5f;
                }
                grid[x][y][z] = density;
            }
        }
    }
}

glm::vec3 MarchingCubes::interpolate(glm::vec3 p1, glm::vec3 p2, float v1, float v2) {
    if (abs(v1 - v2) < 0.0001f) return p1;
    float t = (isoLevel - v1) / (v2 - v1);
    return p1 + t * (p2 - p1);
}

void MarchingCubes::march() {
    vertices.clear();

    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                // get 8 corners of this cube
                glm::vec3 corners[8] = {
                    gridPos(x,   y,   z),
                    gridPos(x+1, y,   z),
                    gridPos(x+1, y,   z+1),
                    gridPos(x,   y,   z+1),
                    gridPos(x,   y+1, z),
                    gridPos(x+1, y+1, z),
                    gridPos(x+1, y+1, z+1),
                    gridPos(x,   y+1, z+1)
                };

                float vals[8] = {
                    grid[x][y][z],
                    grid[x+1][y][z],
                    grid[x+1][y][z+1],
                    grid[x][y][z+1],
                    grid[x][y+1][z],
                    grid[x+1][y+1][z],
                    grid[x+1][y+1][z+1],
                    grid[x][y+1][z+1]
                };

                // build index into lookup table
                int cubeIndex = 0;
                for (int i = 0; i < 8; i++)
                    if (vals[i] < isoLevel) cubeIndex |= (1 << i);

                if (edgeTable[cubeIndex] == 0) continue;

                // interpolate edge vertices
                glm::vec3 edgeVerts[12];
                if (edgeTable[cubeIndex] & 1)    edgeVerts[0]  = interpolate(corners[0], corners[1], vals[0], vals[1]);
                if (edgeTable[cubeIndex] & 2)    edgeVerts[1]  = interpolate(corners[1], corners[2], vals[1], vals[2]);
                if (edgeTable[cubeIndex] & 4)    edgeVerts[2]  = interpolate(corners[2], corners[3], vals[2], vals[3]);
                if (edgeTable[cubeIndex] & 8)    edgeVerts[3]  = interpolate(corners[3], corners[0], vals[3], vals[0]);
                if (edgeTable[cubeIndex] & 16)   edgeVerts[4]  = interpolate(corners[4], corners[5], vals[4], vals[5]);
                if (edgeTable[cubeIndex] & 32)   edgeVerts[5]  = interpolate(corners[5], corners[6], vals[5], vals[6]);
                if (edgeTable[cubeIndex] & 64)   edgeVerts[6]  = interpolate(corners[6], corners[7], vals[6], vals[7]);
                if (edgeTable[cubeIndex] & 128)  edgeVerts[7]  = interpolate(corners[7], corners[4], vals[7], vals[4]);
                if (edgeTable[cubeIndex] & 256)  edgeVerts[8]  = interpolate(corners[0], corners[4], vals[0], vals[4]);
                if (edgeTable[cubeIndex] & 512)  edgeVerts[9]  = interpolate(corners[1], corners[5], vals[1], vals[5]);
                if (edgeTable[cubeIndex] & 1024) edgeVerts[10] = interpolate(corners[2], corners[6], vals[2], vals[6]);
                if (edgeTable[cubeIndex] & 2048) edgeVerts[11] = interpolate(corners[3], corners[7], vals[3], vals[7]);

                // add triangles
                for (int i = 0; triTable[cubeIndex][i] != -1; i += 3) {
                    vertices.push_back(edgeVerts[triTable[cubeIndex][i]]);
                    vertices.push_back(edgeVerts[triTable[cubeIndex][i+1]]);
                    vertices.push_back(edgeVerts[triTable[cubeIndex][i+2]]);
                }
            }
        }
    }
}

void MarchingCubes::update(const std::vector<Particle>& particles) {
    buildGrid(particles);
    march();
}

std::vector<glm::vec3>& MarchingCubes::getVertices() {
    return vertices;
}