#pragma once
#include "Shader.h"
#include "Camera.h"

class Renderer {
public:
    Renderer();
    void draw(Camera& camera, int screenWidth, int screenHeight);

private:
    unsigned int VAO, VBO, EBO;
    Shader shader;
};