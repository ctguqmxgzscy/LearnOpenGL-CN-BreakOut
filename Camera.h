#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

//与Breakout小游戏有冲突，暂时注释掉

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP
};


class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
    Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float CameraSpeed = 25.0f;
    float MouseSensitivity = 0.05f;
    float g = 9.8;

    glm::mat4 GetViewMatrix();
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

private:
    void updateCameraVectors();
};

