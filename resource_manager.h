#pragma once
#include <map>
#include <string>

#include <GL/glew.h>

#include "GameTexture.h"
#include "GameShader.h"


class ResourceManager
{
public:
    static std::map<std::string, GameShader>Shaders;
    static std::map<std::string, Texture2D> Textures;

    static GameShader   LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);

    static GameShader   GetShader(std::string name);

    static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, std::string name);

    static Texture2D GetTexture(std::string name);

    static void      Clear();
private:
    
    ResourceManager() { }

    static GameShader    loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);

    static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);
};

