#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class ICamera
{
public:
    explicit ICamera(const glm::mat4& View) : View(View) { }
    virtual ~ICamera() = default;
    
    glm::mat4& GetView() { return View;}

    float GetFOV() const { return FOV; }
    
    // API
    virtual void ProcessInput(GLFWwindow *Window, float DeltaTime) = 0;
    virtual void Update(float DeltaTime) = 0;
    
protected:
    glm::mat4 View = {};
    float FOV = 45.0f;
};
