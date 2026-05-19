#include "OrbitCamera.h"

#include <GLFW/glfw3.h>

void OrbitCamera::Update(GLFWwindow* window, float deltaTime)
{
    const float rotationSpeed = 70.0f;
    const float zoomSpeed = 6.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        m_yaw += rotationSpeed * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        m_yaw -= rotationSpeed * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        m_pitch += rotationSpeed * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        m_pitch -= rotationSpeed * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_distance -= zoomSpeed * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_distance += zoomSpeed * deltaTime;
    }

    if (m_pitch < 10.0f)
    {
        m_pitch = 10.0f;
    }

    if (m_pitch > 85.0f)
    {
        m_pitch = 85.0f;
    }

    if (m_distance < 6.0f)
    {
        m_distance = 6.0f;
    }

    if (m_distance > 22.0f)
    {
        m_distance = 22.0f;
    }
}
void OrbitCamera::Apply() const
{
    glTranslatef(0.0f, 0.0f, -m_distance);
    glRotatef(m_pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(m_yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, -0.5f, 0.0f);
}   