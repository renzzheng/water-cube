// OpenGL drawing and rendering logic
#include <iostream>
#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// all 8 corners of the cube, centered at the origin with side length 2
float vertices[] = {
    // x      y      z
    -1.0f, -1.0f, -1.0f, // corner 0
    1.0f, -1.0f, -1.0f, // corner 1
    1.0f, 1.0f, -1.0f, // corner 2
    -1.0f, 1.0f, -1.0f, // corner 3
    -1.0f, -1.0f, 1.0f, // corner 4
    1.0f, -1.0f, 1.0f, // corner 5
    1.0f, 1.0f, 1.0f, // corner 6
    -1.0f, 1.0f, 1.0f // corner 7
};

unsigned int indices[] = {
    // bottom face (corners 0,1,2,3)
    0, 1,  // front bottom edge
    1, 2,  // right bottom edge
    2, 3,  // back bottom edge
    3, 0,  // left bottom edge

    // top face (corners 4,5,6,7)
    4, 5,  // front top edge
    5, 6,  // right top edge
    6, 7,  // back top edge
    7, 4,  // left top edge

    // vertical edges connecting bottom to top
    0, 4,  // front left edge
    1, 5,  // front right edge
    2, 6,  // back right edge
    3, 7   // back left edge
};

Renderer::Renderer(): shader("../src/shaders/cube.vert", "../src/shaders/cube.frag"),
                         particleShader("../src/shaders/fluid.vert", "../src/shaders/fluid.frag") {
    // create GPU buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind VAO first
    glBindVertexArray(VAO);

    // upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // tell OpenGL how to read the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind
    glBindVertexArray(0);

    // particle buffers
    glGenVertexArrays(1, &particleVAO);
    glGenBuffers(1, &particleVBO);

    glBindVertexArray(particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferData(GL_ARRAY_BUFFER, 256 * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW); // pre-allocate for 256 particles
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::draw(Camera& camera, int screenWidth, int screenHeight) {
    shader.use();

    // model matrix - identity for now (cube sits at origin)
    glm::mat4 model = glm::mat4(1.0f);

    // view and projection from camera
    glm::mat4 view = camera.getViewMatrix();
    float aspect = (float)screenWidth / (float)screenHeight;
    glm::mat4 projection = camera.getProjectionMatrix(aspect);

    // send matrices to shader
    shader.setMat4("model",      glm::value_ptr(model));
    shader.setMat4("view",       glm::value_ptr(view));
    shader.setMat4("projection", glm::value_ptr(projection));

    // draw the wireframe cube
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::drawParticles(SPHSystem& sph, Camera& camera, int screenWidth, int screenHeight) {
    // extract positions
    std::vector<Particle> particles = sph.getParticleData();
    std::cout << "Drawing " << particles.size() << " particles" << std::endl;
    std::vector<float> positions;
    for (Particle& p : particles) {
        positions.push_back(p.position.x);
        positions.push_back(p.position.y);
        positions.push_back(p.position.z);
    }

    // upload to GPU
    glBindVertexArray(particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);

    // draw
    particleShader.use();

    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = camera.getViewMatrix();
    float aspect         = (float)screenWidth / (float)screenHeight;
    glm::mat4 projection = camera.getProjectionMatrix(aspect);

    particleShader.setMat4("model",      glm::value_ptr(model));
    particleShader.setMat4("view",       glm::value_ptr(view));
    particleShader.setMat4("projection", glm::value_ptr(projection));

    glPointSize(5.0f);
    glBindVertexArray(particleVAO);
    glDrawArrays(GL_POINTS, 0, particles.size());
    glBindVertexArray(0);
}
