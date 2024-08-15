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
#include "Source/NWindow.h"
#include "Source/Camera/NFreeCamera.h"
#include "Source/Camera/NOrbitalCamera.h"
#include "Source/Primitives/NCube.h"

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

void ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char* argv[])
{
    NWindow Window { SCR_WIDTH, SCR_HEIGHT, "Nycsi Engine" };
    
    // Init GLAD before we call any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Our lonely geometry
    NCube Cube;

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
    
    // Create and activate the Shader
    const NShader Shader = NShader("shaders\\shader.vs", "shaders\\shader.fs");
    Shader.Use();
    Shader.SetInt("texture1", 0);
    Shader.SetInt("texture2", 1);
    
    // Enable Depth test
    glEnable(GL_DEPTH_TEST);

    // Create some textures
    const NTexture Texture0 {"textures/container.jpg", GL_TEXTURE0, false};
    const NTexture Texture1 {"textures/awesomeface.png", GL_TEXTURE1, true};
    
    // Set textures
    Texture0.BindAndActivate();
    Texture1.BindAndActivate();
    
    // And we form all the vectors that from the view/camera space
    //NOrbitalCamera Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    NFreeCamera Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Window.SetOnMouseMoveCallback([&Camera](const float X, const float Y)
    {
        Camera.Look(X, Y);
    });

    Window.SetOnMouseScrollCallback([&Camera, &Shader](const float X, const float Y)
    {
        Camera.Zoom(X, Y);
        glm::mat4 Projection = glm::perspective(glm::radians(Camera.GetFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
        Shader.SetMat4("projection", Projection);
    });
    
    // TODO The projection matrix should came from the camera
    // We should have ortographic cameras as well in the future, but for now this should work
    glm::mat4 Projection = glm::perspective(glm::radians(Camera.GetFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
    Shader.SetMat4("projection", Projection);
    
    // Time of last frame
    float LastFrame = 0.0f;
    
    // Render Loop
    while (!Window.ShouldClose())
    {
        const float CurrentFrame = glfwGetTime();
        const float DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;
        
        // Input
        ProcessInput(Window.GetGlfWindow());
        Camera.ProcessInput(Window.GetGlfWindow(), DeltaTime);
        
        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // And for this previously set color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Color and Depth test

        Camera.Update(DeltaTime);
        Shader.SetMat4("view", Camera.GetView());
        
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, CubePositions[i]);
            Model = glm::rotate(Model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            Shader.SetMat4("model", Model);
            Cube.Draw();
        }
        
        // Check and call events, Swap the buffers
        Window.SwapBuffers();
        glfwPollEvents();
    }
    
    // Nicely clean/delete all allocated GLFW resources
    glfwTerminate();
    
    return 0;
}
