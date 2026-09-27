#include "Application.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <numbers>

namespace
{
float Radians(float degrees)
{
    return degrees * std::numbers::pi_v<float> / 180.0f;
}

Transform CreateTransform()
{
    Transform transform;
    transform.rotation = { Radians(15.0f), Radians(25.0f), 0.0f };
    transform.scale = { 1.0f, 1.0f, 1.0f };
    return transform;
}

}

Application::Application()
    : m_window(1000, 700, "Task 3.7 - Octahedron")
    , m_shape(CreateTransform())
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
