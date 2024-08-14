#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Source/NShader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Source/NTexture.h"

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

    // Geometry
    const float vertices[] =
    {
        // positions      // colors            // texture coords
        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,        // top right
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,        // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,        // bottom left
        -0.5f,0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f         // top left
    };

    unsigned int indices[] =
    {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    // Generate the Vertex Buffer Object and Vertex Array Object 
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 1. bind Vertex Array Object.
    glBindVertexArray(VAO);

    // 2. Copy VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Bind the element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 3. then set our vertex attributes pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute. We need to set an offset
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    const NTexture Texture0 {"textures/container.jpg", GL_TEXTURE0, false};
    const NTexture Texture1 {"textures/awesomeface.png", GL_TEXTURE1, true};
    
    // Activate the shader program. As we have only one shader
    Shader.Use();
    Shader.SetInt("texture1", 0);
    Shader.SetInt("texture2", 1);
    
    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        ProcessInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // And for this previously set color
        glClear(GL_COLOR_BUFFER_BIT); // We care about the color buffer
        
        // Set textures
        Texture0.BindAndActivate();
        Texture1.BindAndActivate();
        
        Shader.Use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // Check and call events, Swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    // Nicely clean/delete all allocated GLFW resources
    glfwTerminate();
    
    return 0;
}
