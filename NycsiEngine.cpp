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
#include "glm/gtx/rotate_vector.hpp"
#include "Lighting/NLight.h"
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

    // Geometry Definition
    NCube Cube;
    NLight Light;
    
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

    glm::vec3 LightPos(2.0f, 2.0f, 2.0f);

    NFreeCamera Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Create Light Shader
    const NShader LightShader = NShader("shaders\\shader.vert", "shaders\\lightShader.frag");
    
    // Create and activate the Shader
    const NShader Shader = NShader("shaders\\shader.vert", "shaders\\shader.frag");
    Shader.Use();
    Shader.SetInt("texture1", 0);
    Shader.SetInt("texture2", 1);
    Shader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
    Shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    Shader.SetVec3("lightPos", LightPos);
    
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
    
    
    Window.SetOnMouseMoveCallback([&Camera](const float X, const float Y)
    {
        Camera.Look(X, Y);
    });

    // We probably need a list of Shaders, or some container of shaders for the future
    // Something to link the shader to update it when we need it
    Window.SetOnMouseScrollCallback([&Camera, &Shader, &LightShader](const float X, const float Y)
    {
        Camera.Zoom(X, Y);

        Shader.Use();
        Shader.SetMat4("projection", Camera.GetProjection());

        LightShader.Use();
        LightShader.SetMat4("projection", Camera.GetProjection());
        
    });
    
    // We should have ortographic cameras as well in the future, but for now this should work
    Shader.SetMat4("projection", Camera.GetProjection());
    
    // Set up the Light
    {
        LightShader.Use();
        glm::mat4 Model = glm::mat4(1.0f);
        Model = glm::translate(Model, LightPos);
        Model = glm::scale(Model, glm::vec3(0.2f));
        LightShader.SetMat4("model", Model);
        LightShader.SetMat4("projection", Camera.GetProjection());
        
    }
    
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
        Camera.Update(DeltaTime);
        
        // Clear the scene
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // And for this previously set color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Color and Depth test
        
        // Cube Rendering
        /*
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, CubePositions[i]);
            Model = glm::rotate(Model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            Shader.SetMat4("model", Model);
            Shader.Use();
            Cube.Draw();
        }
        */

        // Cube Rendering
        {
            Shader.Use();
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::rotate(Model,  glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            Shader.SetMat4("view", Camera.GetView());
            Shader.SetMat4("model", Model);
            Shader.SetVec3("viewPos", Camera.GetPosition());
            Cube.Draw();    
        }

        // Light Drawing
        {
            LightShader.Use();
            LightShader.SetMat4("view", Camera.GetView());
            Light.Draw();    
        }
        
        
        // Check and call events, Swap the buffers
        Window.SwapBuffers();
        glfwPollEvents();
    }
    
    // Nicely clean/delete all allocated GLFW resources
    glfwTerminate();
    
    return 0;
}
