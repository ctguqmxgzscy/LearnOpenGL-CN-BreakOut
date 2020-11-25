#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"GameObject.h"
#include"GameTexture.h"

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
    // PowerUp State
    std::string Type;
    GLfloat     Duration;
    GLboolean   Activated;
    // Constructor
    PowerUp(std::string type, glm::vec3 color, GLfloat duration,
        glm::vec2 position, Texture2D texture)
        : GameObject(position, SIZE, texture, color, VELOCITY),
        Type(type), Duration(duration), Activated()
    { }
};

