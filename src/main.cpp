// game loop, window setup
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "rendering/Renderer.h"
#include "rendering/Camera.h"
#include "simulation/SPHSystem.h"
#include "simulation/MarchingCubes.h"
#include <glm/gtc/matrix_transform.hpp>

const int WIDTH  = 800;
const int HEIGHT = 600;

Camera camera;

// mouse tracking
bool  firstMouse = true;
float lastX = WIDTH  / 2.0f;
float lastY = HEIGHT / 2.0f;

// orbit camera around the cube when left mouse button is held and mouse moves
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float deltaX =  (xpos - lastX) * 0.3f;
    float deltaY = (ypos - lastY) * 0.3f;

    lastX = xpos;
    lastY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        camera.orbit(deltaX, deltaY);
    }
}


int main() {
    // initialize GLFW only once
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // initilaization hints to let GLFW know we want OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // required to run on macos

    // window creation
    GLFWwindow* window = glfwCreateWindow(600, 400, "Water Cube", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouseCallback);

    // load in OpenGL functions via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // error handling
    // int code = glfwGetError(NULL);
    // if (code != GLFW_NO_ERROR)
    //     handle_error(code);

    // enable depth testing so closer faces draw on top
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);  // cull front instead of back
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Renderer renderer;
    SPHSystem sph;

    MarchingCubes mc(10, 2.0f);

    // main loop that runs with the water cube
    float lastFrame = 0.0f;

    // rotation matrix
    glm::mat4 cubeRotation = glm::mat4(1.0f); // starts as identity (no rotation)
    float rotateSpeed = 1.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        dt = std::min(dt, 0.016f);

        sph.update(dt);

        // cube rotation input
        if (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS)
            cubeRotation = glm::rotate(cubeRotation, rotateSpeed * dt, glm::vec3(0, 0, 1));
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            cubeRotation = glm::rotate(cubeRotation, -rotateSpeed * dt, glm::vec3(0, 0, 1));
        if (glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS)
            cubeRotation = glm::rotate(cubeRotation, rotateSpeed * dt, glm::vec3(1, 0, 0));
        if (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS)
            cubeRotation = glm::rotate(cubeRotation, -rotateSpeed * dt, glm::vec3(1, 0, 0));

        // update gravity direction based on cube rotation
        glm::vec3 worldGravity = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::mat3 invRotation = glm::mat3(glm::transpose(cubeRotation));
        glm::vec3 localGravity = invRotation * worldGravity;
        sph.setGravityDirection(localGravity);

        // update simulation
        sph.update(dt);
        mc.update(sph.getParticleData());

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.draw(camera, WIDTH, HEIGHT, cubeRotation);
        renderer.drawFluidSurface(mc, camera, WIDTH, HEIGHT, cubeRotation);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // terminate GLFW when done to destroy the window and clean up resources
    glfwTerminate();
    return 0;
}