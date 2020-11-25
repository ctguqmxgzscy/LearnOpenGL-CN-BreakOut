#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameTexture.h"
#include "GameShader.h"


class SpriteRenderer
{
public:
    SpriteRenderer(const GameShader& shader);
    ~SpriteRenderer();
    // 用已给渲染精灵渲染纹理
    void DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    GameShader shader;
    GLuint quadVAO;
    void initRenderData();
};