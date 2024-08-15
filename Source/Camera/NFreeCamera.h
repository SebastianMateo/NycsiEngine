#pragma once
#include "ICamera.h"
#include "glm/glm.hpp"

class NFreeCamera final : public ICamera
{
public:
    NFreeCamera(glm::vec3 Pos, glm::vec3 Front, glm::vec3 Up);
    ~NFreeCamera() override = default;

    
    void ProcessInput(GLFWwindow* Window, float DeltaTime) override;
    void Update(float DeltaTime) override;

    // Look with Mouse X and Y Coordinates
    void Look(double X, double Y);
    void Zoom(float XOffset, float YOffset);

private:
    glm::vec3 Pos = {};
    glm::vec3 Front = {};
    glm::vec3 Up = {};

    // TODO Fix this. Center of the screen. Should come as parameter
    double LastX = 400;
    double LastY = 300;
    bool FirstMouse = true;

    double Yaw = -90;
    double Pitch = 0;

    // TODO Should be able to customize
    float Sensitivity = 0.1f;
    static constexpr float CameraSpeed = 2.5f;
};
