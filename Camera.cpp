#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
    Position = position;
    WorldUp = worldup;
    Front = glm::normalize(position - target);
    Right = glm::normalize(glm::cross(WorldUp, Front));
    Up = glm::normalize(glm::cross(Front, Right));
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
    Position = position;
    WorldUp = worldup;
    Pitch = pitch;
    Yaw = yaw;
    Front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw)); 
    Front.y = sin(glm::radians(pitch));
    Front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    Right = glm::normalize(glm::cross(WorldUp, Front));
    Up = glm::normalize(glm::cross(Front, Right));
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));  
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = CameraSpeed * deltaTime;

    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
  //  Position.y = 0;
}


