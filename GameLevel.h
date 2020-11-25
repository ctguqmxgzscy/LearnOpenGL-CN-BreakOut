#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "SpriteRender.h"
#include "resource_manager.h"



class GameLevel
{
public:
    // 关卡状态
    std::vector<GameObject> Bricks;
    GameLevel() { }
    // 从文件中加载关卡
    void      Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
    // 渲染关卡
    void      Draw(SpriteRenderer& renderer);
    // 检查一个关卡是否已完成 (所有非坚硬的瓷砖均被摧毁)
    GLboolean IsCompleted();
private:
    // Initialize level from tile data
    void      init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};