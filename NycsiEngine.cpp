#include <iostream>
#include <windows.h>


#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "Source/Rendering/NShader.h"
#include "Source/Rendering/NTexture.h"

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

    glm::vec3 PointLightPositions[] =
    {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    NFreeCamera Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Create Light Shader
    const NShader LightShader = NShader("shaders\\shader.vert", "shaders\\lightShader.frag");

    // Create some textures
    const NTexture DiffuseTexture {"textures/containerDiffuse.png", GL_TEXTURE0, ETextureType::Diffuse, true};
    const NTexture SpecularTexture {"textures/containerSpecular.png", GL_TEXTURE1, ETextureType::Specular, true};
    
    // Create and activate the Shader
    const NShader Shader = NShader("shaders\\shader.vert", "shaders\\shader.frag");
    Shader.Use();

    // Set Materials
    DiffuseTexture.BindAndActivate();
    SpecularTexture.BindAndActivate();
    Shader.SetInt("material.diffuse", 0);
    Shader.SetInt("material.specular", 1);
    Shader.SetFloat("material.shininess", 32.0f);

    // Set all the lights // TODO IMPROVE
    // directional light
    Shader.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    Shader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    Shader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    Shader.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    Shader.SetVec3("pointLights[0].position", PointLightPositions[0]);
    Shader.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    Shader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    Shader.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    Shader.SetFloat("pointLights[0].constant", 1.0f);
    Shader.SetFloat("pointLights[0].linear", 0.09f);
    Shader.SetFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    Shader.SetVec3("pointLights[1].position", PointLightPositions[1]);
    Shader.SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    Shader.SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    Shader.SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    Shader.SetFloat("pointLights[1].constant", 1.0f);
    Shader.SetFloat("pointLights[1].linear", 0.09f);
    Shader.SetFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    Shader.SetVec3("pointLights[2].position", PointLightPositions[2]);
    Shader.SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    Shader.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    Shader.SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    Shader.SetFloat("pointLights[2].constant", 1.0f);
    Shader.SetFloat("pointLights[2].linear", 0.09f);
    Shader.SetFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    Shader.SetVec3("pointLights[3].position", PointLightPositions[3]);
    Shader.SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    Shader.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    Shader.SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    Shader.SetFloat("pointLights[3].constant", 1.0f);
    Shader.SetFloat("pointLights[3].linear", 0.09f);
    Shader.SetFloat("pointLights[3].quadratic", 0.032f);

    // spotLight
    Shader.SetVec3("spotLight.position", Camera.GetPosition());
    Shader.SetVec3("spotLight.direction", Camera.GetFront());
    Shader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    Shader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    Shader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    Shader.SetFloat("spotLight.constant", 1.f);
    Shader.SetFloat("spotLight.linear", 0.09f);
    Shader.SetFloat("spotLight.quadratic", 0.032f);
    Shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(8.f)));
    Shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.0f)));     
    
    // Enable Depth test
    glEnable(GL_DEPTH_TEST);
    
    // And we form all the vectors that from the view/camera space
    //NOrbitalCamera Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    Window.SetOnMouseMoveCallback([&Camera, &Shader](const float X, const float Y)
    {
        Camera.Look(X, Y);

        Shader.Use();
        Shader.SetVec3("spotLight.position", Camera.GetPosition());
        Shader.SetVec3("spotLight.direction", Camera.GetFront());
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
    Shader.Use();
    Shader.SetMat4("projection", Camera.GetProjection());

    LightShader.Use();
    LightShader.SetMat4("projection", Camera.GetProjection());

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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // And for this previously set color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Color and Depth test
        
        // Cube Rendering
        for(unsigned int i = 0; i < 10; i++)
        {
            Shader.Use();
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, CubePositions[i]);
            Model = glm::rotate(Model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            Shader.SetVec3("spotLight.position", Camera.GetPosition());
            Shader.SetVec3("spotLight.direction", Camera.GetFront());
            Shader.SetMat4("view", Camera.GetView());
            Shader.SetMat4("model", Model);
            Shader.SetVec3("viewPos", Camera.GetPosition());
            Cube.Draw();
        }

        for (glm::vec3 PointLightPosition : PointLightPositions)
        {
            LightShader.Use();
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, PointLightPosition);
            Model = glm::scale(Model, glm::vec3(0.2f));
            LightShader.SetMat4("view", Camera.GetView());
            LightShader.SetMat4("model", Model);
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
