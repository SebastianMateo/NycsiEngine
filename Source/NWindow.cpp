#include "NWindow.h"

#include <iostream>

NWindow::NWindow(const int ScrWidth, const int ScrHeight, const std::string& Title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Window Create
    GlfwWindow = glfwCreateWindow(ScrWidth, ScrHeight, Title.c_str(), nullptr, nullptr);
    if (GlfwWindow == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(GlfwWindow);

    glfwSetWindowUserPointer(GlfwWindow, this);
    
    // Callback when Resizing the Window
    glfwSetFramebufferSizeCallback(GlfwWindow, [](GLFWwindow* Window, const int Width, const int Height)
    {
        static_cast<NWindow*>(glfwGetWindowUserPointer(Window))->FramebufferSizeCallback(Window, Width, Height);
    });

    // Mouse callback
    glfwSetCursorPosCallback(GlfwWindow, [](GLFWwindow* Window, const double X, const double Y)
    {
        static_cast<NWindow*>(glfwGetWindowUserPointer(Window))->MouseMoveCallback(Window, X, Y);
    });

    // Mouse callback
    glfwSetScrollCallback(GlfwWindow, [](GLFWwindow* Window, const double XOffset, const double YOffset)
    {
        static_cast<NWindow*>(glfwGetWindowUserPointer(Window))->MouseScrollCallback(Window, XOffset, YOffset);
    });

    // Hide the cursor and capture it
    glfwSetInputMode(GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
}

void NWindow::SwapBuffers() const
{
    glfwSwapBuffers(GlfwWindow);
}

bool NWindow::ShouldClose() const
{
    return glfwWindowShouldClose(GlfwWindow);
}

void NWindow::FramebufferSizeCallback(GLFWwindow* Window, const int Width, const int Height)
{
    glViewport(0, 0, Width, Height);
}

void NWindow::MouseMoveCallback(GLFWwindow* Window, const double X, const double Y) const
{
    // Should probably have a list of handlers.. but for now this should work
    OnMouseMoveCallback(X, Y);
}

void NWindow::MouseScrollCallback(GLFWwindow* Window, double XOffset, double YOffset)
{
    OnMouseScrollCallback(XOffset, YOffset);
}
