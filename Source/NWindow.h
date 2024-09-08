#pragma once
#include <functional>
#include <string>

#include "GLFW/glfw3.h"

class NWindow
{
public:
    NWindow(int ScrWidth, int ScrHeight, const std::string& Title);

    void SwapBuffers() const;
    bool ShouldClose() const;

    GLFWwindow* GetGlfWindow() const { return GlfwWindow;}

    void SetOnMouseMoveCallback(const std::function<void(double, double)>& OnMouseMoveCallback)
    {
        this->OnMouseMoveCallback = OnMouseMoveCallback;
    }

    void SetOnMouseScrollCallback(const std::function<void(double, double)>& OnMouseScrollCallback)
    {
        this->OnMouseScrollCallback = OnMouseScrollCallback;
    }

private:
    GLFWwindow* GlfwWindow;

    // Callbacks
    std::function<void(double, double)> OnMouseMoveCallback;
    std::function<void(double, double)> OnMouseScrollCallback;
    
    static void FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height);
    
    void MouseMoveCallback(GLFWwindow* Window, double X, double Y) const;
    void MouseScrollCallback(GLFWwindow* Window, double XOffset, double YOffset);
};
