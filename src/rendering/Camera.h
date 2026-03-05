#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    float radius;
    float yaw;
    float pitch;
    float fov;
    glm::vec3 target;
    glm::vec3 position;

    Camera();

    void orbit(float deltaYaw, float deltaPitch);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix(float aspectRatio);

private:
    void updatePosition();
};