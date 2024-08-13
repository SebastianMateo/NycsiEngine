#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Source/NShader.h"

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

    const NShader Shader = NShader("shader.vs", "shader.fs");

    // Geometry
    constexpr float vertices[] 
    {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // right  
         -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // left 
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top   
    }; 
    
    // Generate the Vertex Buffer Object and Vertex Array Object 
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 1. bind Vertex Array Object.
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. then set our vertex attributes pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute. We need to set an offset
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // Optional cleanup
    // We can unbind as the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(VAO);

    // Activate the shader program. As we have only one shader
    Shader.Use();
    
    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        ProcessInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // And for this previously set color
        glClear(GL_COLOR_BUFFER_BIT); // We care about the color buffer
        
        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Check and call events, Swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    // Nicely clean/delete all allocated GLFW resources
    glfwTerminate();
    
    return 0;
}
