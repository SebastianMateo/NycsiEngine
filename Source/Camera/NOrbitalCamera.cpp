#include "NOrbitalCamera.h"

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

NOrbitalCamera::NOrbitalCamera(const glm::vec3 Eye, const glm::vec3 Center, const glm::vec3 Up):
    ICamera(glm::lookAt(Eye, Center, Up)),
    Eye(Eye),
    Center(Center),
    Up(Up)
{
}

void NOrbitalCamera::Update(float DeltaTime)
{
    // Create a View Matrix
    constexpr float Radius = 10.0f;
    const float CamX = sin(glfwGetTime()) * Radius * DeltaTime;
    const float CamZ = cos(glfwGetTime()) * Radius * DeltaTime;
    View = glm::lookAt
    (
        glm::vec3(CamX, 0.0f, CamZ),
        Center,
        Up
    );
}

void NOrbitalCamera::ProcessInput(GLFWwindow* window, float DeltaTime)
{
}
