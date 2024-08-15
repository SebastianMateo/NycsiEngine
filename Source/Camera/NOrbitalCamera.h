#pragma once
#include "ICamera.h"
#include "glm/glm.hpp"

class NOrbitalCamera final : public ICamera
{
public:
    NOrbitalCamera(glm::vec3 Eye, glm::vec3 Center, glm::vec3 Up);
    void Update(float DeltaTime) override;
    
    void ProcessInput(GLFWwindow* window, float DeltaTime) override;

private:
    glm::vec3 Eye;
    glm::vec3 Center;
    glm::vec3 Up;
};
