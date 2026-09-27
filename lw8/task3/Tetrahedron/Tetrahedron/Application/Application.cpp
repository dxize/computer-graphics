#include "Application.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <numbers>
#include <array>

namespace
{
float Radians(float degrees)
{
    return degrees * std::numbers::pi_v<float> / 180.0f;
}

Transform CreateTransform()
{
    Transform transform;
    transform.rotation = { Radians(15.0f), Radians(-40.0f), 0.0f };
    transform.scale = { 1.0f, 1.0f, 1.0f };
    return transform;
}

std::array<Vector3, 4> CreateTetrahedronVertices()
{
    return {
        Vector3{ 0.0f, 1.0f, 0.0f },
        Vector3{ -0.9f, -0.8f, 0.8f },
        Vector3{ 0.9f, -0.8f, 0.8f },
        Vector3{ 0.0f, -0.8f, -0.9f }
    };
}
}

Application::Application()
    : m_window(1000, 700, "Task 3.6 - Tetrahedron")
    , m_shape(CreateTetrahedronVertices(), CreateTransform())
{
}

void Application::Run()
{
    while (!m_window.ShouldClose())
    {
        HandleInput();
        RenderFrame();
        m_window.SwapBuffers();
        glfwPollEvents();
    }
}

void Application::HandleInput()
{
    if (glfwGetKey(m_window.GetHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        m_window.Close();
}

void Application::RenderFrame()
{
    m_renderer.Render(m_shape, m_cameraPosition, m_window.GetFramebufferSize());
}
