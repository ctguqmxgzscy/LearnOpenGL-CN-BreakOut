#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include<vector>
#include<algorithm>
#include<string>

#include"resource_manager.h"
#include"GameShader.h"
#include"Mesh.h"
#include "Shader.h"
#include"Camera.h"
#include"Model.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma region 3D  Game Test

//加载立方体贴图函数
GLuint loadCubemap(vector<const GLchar*> faces);
GLuint loadTexture(const char* path);

const int WIDTH = 800, HEIGHT = 600;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

#pragma region Camera Declare
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::radians(-15.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
#pragma endregion

#pragma region Input Declare

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

#pragma endregion

#pragma region Model Data

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
 glm::vec3(0.0f,  0.0f,  0.0f),
 glm::vec3(2.0f,  5.0f, -15.0f),
 glm::vec3(-1.5f, -2.2f, -2.5f),
 glm::vec3(-3.8f, -2.0f, -12.3f),
 glm::vec3(2.4f, -0.4f, -3.5f),
 glm::vec3(-1.7f,  3.0f, -7.5f),
 glm::vec3(1.3f, -2.0f, -2.5f),
 glm::vec3(1.5f,  2.0f, -2.5f),
 glm::vec3(1.5f,  0.2f, -1.5f),
 glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};

GLfloat planeVertices[] = {
    // Positions          // Texture Coords 
     5.0f, -0.5f,  5.0f,  2.0f,  0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f,  0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f,  2.0f,

     5.0f, -0.5f,  5.0f,  2.0f,  0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f,  2.0f,
     5.0f, -0.5f, -5.0f,  2.0f,  2.0f
};
#pragma endregion

int main( int argc,char* argv[])
{

#pragma region Open a window
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

   GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

   glfwMakeContextCurrent(window);
   glfwSetKeyCallback(window, key_callback);
   glfwSetScrollCallback(window, scroll_callback);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPosCallback(window, mouse_callback);

   glEnable(GL_DEPTH_TEST);
   glewExperimental = GL_TRUE;
   glewInit();
   glGetError();
   glViewport(0, 0, WIDTH, HEIGHT);
#pragma endregion

#pragma region Init Shader Program
   Shader myShader("vertexSource.vert", "fragmentSource.frag");
   Shader ContainerShader("Container.vert", "Container.frag");
   Shader lightShader("lightShader.vert", "lightShader.frag");
   Shader ModelShader("ModelShader.vert", "ModelShader.frag");
#pragma endregion

#pragma region  Model Load
    Model myModel("D:/OpenGL/learnOpenGl/Debug/Nanosuit/nanosuit.obj");
#pragma endregion

#pragma region Sky Box
      vector<const GLchar*> faces;
      faces.push_back("right.jpg");
      faces.push_back("left.jpg");
      faces.push_back("top.jpg");
      faces.push_back("bottom.jpg");
      faces.push_back("back.jpg");
      faces.push_back("front.jpg");
      GLuint cubemapTexture = loadCubemap(faces);
  #pragma endregion

#pragma region Sky Box Data
        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
#pragma endregion

#pragma region Init and Load Models to VAO,VBO,SkyBox
   GLuint VBO, containerVAO;
   glGenVertexArrays(1, &containerVAO);
   glGenBuffers(1, &VBO);
    
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   
   glBindVertexArray(containerVAO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
   glEnableVertexAttribArray(1);
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
  // glEnableVertexAttribArray(2);
   glBindVertexArray(0);
   GLuint containerTexture = loadTexture("container.jpg");

   GLuint skyboxVAO, skyboxVBO;
   glGenVertexArrays(1, &skyboxVAO);
   glGenBuffers(1, &skyboxVBO);
   glBindVertexArray(skyboxVAO);
   glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
   glBindVertexArray(0);

   GLuint planeVAO, planeVBO;
   glGenVertexArrays(1, &planeVAO);
   glGenBuffers(1, &planeVBO);
   glBindVertexArray(planeVAO);
   glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
   glBindVertexArray(0);
   GLuint floorTexture = loadTexture("wall.jpg");
#pragma endregion
/*
#pragma region Test Game Loop
   
    GLuint diffuseMap, specularMap, emissionMap;
    glGenTextures(1, &diffuseMap);
    glGenTextures(1, &specularMap);
    glGenTextures(1, &emissionMap);
    int width, height, nrChannels;
    
    unsigned char* image = SOIL_load_image("container2.png", &width, &height, 0, SOIL_LOAD_RGBA);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    
    image = SOIL_load_image("container2_specular.png", &width, &height, &nrChannels, 0);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    
   myShader.use();
   glUniform1i(glGetUniformLocation(myShader.ID, "material.diffuse"), 0);
   glUniform1i(glGetUniformLocation(myShader.ID, "material.specular"), 1);
    
    while (!glfwWindowShouldClose(window))
    {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        myShader.use();
    
        glUniform1f(glGetUniformLocation(myShader.ID, "material.shininess"), 32.0f);
         //平行光
        glUniform3f(glGetUniformLocation(myShader.ID, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(myShader.ID, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(myShader.ID, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(myShader.ID, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
        // 4 个 点光源
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[0].linear"), 0.09);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[0].quadratic"), 0.032);
    
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[1].linear"), 0.09);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[1].quadratic"), 0.032);
    
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[2].linear"), 0.09);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[2].quadratic"), 0.032);
    
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(myShader.ID, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[3].linear"), 0.09);
        glUniform1f(glGetUniformLocation(myShader.ID, "pointLights[3].quadratic"), 0.032);
        //手电筒
        glUniform3f(glGetUniformLocation(myShader.ID, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform3f(glGetUniformLocation(myShader.ID, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
        glUniform3f(glGetUniformLocation(myShader.ID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(myShader.ID, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(myShader.ID, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(myShader.ID, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(myShader.ID, "spotLight.linear"), 0.09);
        glUniform1f(glGetUniformLocation(myShader.ID, "spotLight.quadratic"), 0.032);
        glUniform1f(glGetUniformLocation(myShader.ID, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(myShader.ID, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
    
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        myShader.use();
    
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // Bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
    
    
        glm::mat4 model;
        glBindVertexArray(containerVAO);
        for (GLuint i = 0; i < 10; i++)
        {
            model = glm::mat4();
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20 * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            myShader.use();
            glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
    
        }
        myModel.Draw(myShader);
        glBindVertexArray(0);
    
       
    
        //Prepare for next
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
#pragma endregion

    glm::vec3 pointLightPositions[] = {
       glm::vec3(2.3f, -1.6f, -3.0f),
       glm::vec3(-1.7f, 0.9f, 1.0f)
   };
*/
     
   while (!glfwWindowShouldClose(window))
    {

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glDepthMask(GL_FALSE);

        myShader.use();
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(myShader.ID, "skybox"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);

        ContainerShader.use();
        glm::mat4 model;
        view = camera.GetViewMatrix();

        glUniformMatrix4fv(glGetUniformLocation(ContainerShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(ContainerShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ContainerShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(containerVAO);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(ContainerShader.ID, "ContainerTex"), 0);
        glBindTexture(GL_TEXTURE_2D, containerTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        
        ModelShader.use(); 
          // Transformation matrices
        projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(ModelShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(ModelShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 Model;
        Model = glm::translate(Model, glm::vec3(0.0f, -2.5f, 10.0f)); 
        Model = glm::scale(Model, glm::vec3(0.4f, 0.4f, 0.4f));	
        glUniformMatrix4fv(glGetUniformLocation(ModelShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(Model));
        myModel.Draw(ModelShader);
        
        ContainerShader.use();
        model = glm::translate(model, glm::vec3(0, 0, 10.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(ContainerShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(ContainerShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ContainerShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(ContainerShader.ID, "ContainerTex"), 1);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    

}

#pragma region  LoadTexture 2D or 3D
    GLuint loadCubemap(vector<const GLchar*> faces)
    {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);

        int width, height;
        unsigned char* image;

        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        for (GLuint i = 0; i < faces.size(); i++)
        {
            image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGBA);
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image
            );
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        return textureID;
    }

    GLuint loadTexture(const char* path)
    {
    
        GLuint textureID;
        glGenTextures(1, &textureID);
        int width, height;
        unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
    
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        SOIL_free_image_data(image);
        return textureID;
    }
#pragma endregion
  


#pragma endregion
/*
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint SCREEN_WIDTH = 800;

const GLuint SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); 

    glfwSetKeyCallback(window, key_callback);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Breakout.Init();

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    Breakout.State = GAME_MENU;

    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();


        Breakout.ProcessInput(deltaTime);

        Breakout.Update(deltaTime);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Render();

        glfwSwapBuffers(window);
    }

    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            Breakout.Keys[key] = GL_FALSE;
    }
}*/