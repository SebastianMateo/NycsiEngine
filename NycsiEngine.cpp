#include <iostream>
#include <windows.h>


#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "Source/NShader.h"
#include "Source/NTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Source/Primitives/NCube.h"

void FramebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window Create
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Nycsi Engine", NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Set the callback when resizing the windows
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    
    // Init GLAD before we call any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    const NShader Shader = NShader("shaders\\shader.vs", "shaders\\shader.fs");
    NCube Cube;
    
    const NTexture Texture0 {"textures/container.jpg", GL_TEXTURE0, false};
    const NTexture Texture1 {"textures/awesomeface.png", GL_TEXTURE1, true};

    // Create a Model Matrix
    //glm::mat4 Model = glm::mat4(1.0f);
    //Model = glm::rotate(Model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // Create a View Matrix
    glm::mat4 View = glm::mat4(1.0f);
    View = glm::translate(View, glm::vec3(0.0f, 0.0f, -3.0f));

    // Create a Projection Matrix
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    // Activate the shader program. As we have only one shader
    Shader.Use();
    Shader.SetInt("texture1", 0);
    Shader.SetInt("texture2", 1);
    //Shader.SetMat4("model", Model);
    Shader.SetMat4("view", View);
    Shader.SetMat4("projection", Projection);
    
    // Enable Depth test
    glEnable(GL_DEPTH_TEST);

    constexpr glm::vec3 CubePositions[]
    {
        glm::vec3( 0.0f,0.0f,0.0f),
        glm::vec3( 2.0f,5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f,-2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f,-3.5f),
        glm::vec3(-1.7f,3.0f,-7.5f),
        glm::vec3( 1.3f, -2.0f,-2.5f),
        glm::vec3( 1.5f,2.0f,-2.5f),
        glm::vec3( 1.5f,0.2f,-1.5f),
        glm::vec3(-1.3f,1.0f,-1.5f)
    };

    // Set textures
    Texture0.BindAndActivate();
    Texture1.BindAndActivate();
    
    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        ProcessInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // And for this previously set color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Color and Depth test
        
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, CubePositions[i]);
            Model = glm::rotate(Model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            Shader.SetMat4("model", Model);
            Cube.Draw();
        }
        
        // Check and call events, Swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Nicely clean/delete all allocated GLFW resources
    glfwTerminate();
    
    return 0;
}
