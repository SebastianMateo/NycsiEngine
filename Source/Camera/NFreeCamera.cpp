#include "NFreeCamera.h"

#include "glm/ext/matrix_transform.hpp"

NFreeCamera::NFreeCamera(const glm::vec3 Pos, const glm::vec3 Front, const glm::vec3 Up):
    ICamera(glm::lookAt(Pos, Pos + Front, Up)),
    Pos(Pos),
    Front(Front),
    Up(Up)
{
}

void NFreeCamera::Look(const double X, const double Y)
{
    // Avoid sharp turn on first time
    if (FirstMouse)
    {
        LastX = X;
        LastY = Y;
        FirstMouse = false;
    }
    
    double XOffset = X - LastX;
    double YOffset = LastY - Y;
    LastX = X;
    LastY = Y;
    
    XOffset *= Sensitivity;
    YOffset *= Sensitivity;
    
    Yaw += XOffset;
    Pitch += YOffset;
    
    // We don't really want to look Straight UP or Down, it would break our system
    if(Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;
    
    glm::vec3 Direction;
    Direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Direction.y = sin(glm::radians(Pitch));
    Direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(Direction);
}

void NFreeCamera::ProcessInput(GLFWwindow* Window, float DeltaTime)
{
    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
        Pos += CameraSpeed * Front * DeltaTime;
    if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
        Pos -= CameraSpeed * Front * DeltaTime;
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
        Pos -= glm::normalize(glm::cross(Front, Up)) * CameraSpeed * DeltaTime;
    if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
        Pos += glm::normalize(glm::cross(Front, Up)) * CameraSpeed * DeltaTime;
}

void NFreeCamera::Update(float DeltaTime)
{
    View = glm::lookAt(Pos, Pos + Front, Up);
}

void NFreeCamera::Zoom(float XOffset, const float YOffset)
{
    FOV -= YOffset;
    if (FOV < 1.0f)
        FOV = 1.0f;
    
    if (FOV > 45.0f)
        FOV = 45.0f;
}
