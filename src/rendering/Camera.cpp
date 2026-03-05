// view + rotation + projection handling
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

// initialize camera parameters
Camera::Camera() {
    radius = 3.0f;
    yaw = -90.0f;
    pitch = 0.0f;
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    fov = 75.0f;
    updatePosition();
}

void Camera::orbit(float deltaYaw, float deltaPitch) {
    yaw   += deltaYaw;
    pitch += deltaPitch;

    // clamp pitch so camera doesn't flip upside down
    if (pitch >  89.0f) pitch =  89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updatePosition();
}

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) {
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
}

// compute the position of the camera in world space based on the spherical coordinates (radius, yaw, pitch)
void Camera::updatePosition() {
    position.x = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    position.y = radius * sin(glm::radians(pitch));
    position.z = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
}