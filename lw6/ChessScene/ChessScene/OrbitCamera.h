#pragma once

#include <GLFW/glfw3.h>

class OrbitCamera
{
public:
    void Update(GLFWwindow* window, float deltaTime);
    void Apply() const;

private:
    float m_yaw = 45.0f;
    float m_pitch = 45.0f;
    float m_distance = 12.0f;
};