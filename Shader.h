#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include<string>
#include<iostream>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:	
	Shader(const char* vertexPath, const char* fragmentPath);
	std::string vertexString;
	std::string fragmentString;
	const char* vertexSource;
	const char* fragmentSource;
	
	unsigned int ID;
	void use();
	void SetUniform3f(const char* paramNameString, glm::vec3 param);
	void SetUnifrom1f(const char* paramNameString, float param);
	void SetUniform1i(const char* paramNameString, int slot);

private:
	void checkCompileErrors(unsigned int ID, std::string type);
};

