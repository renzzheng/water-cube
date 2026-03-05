// OpenGL drawing and rendering logic
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

Renderer::Renderer() : shader("../src/shaders/cube.vert", "../src/shaders/cube.frag") {
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