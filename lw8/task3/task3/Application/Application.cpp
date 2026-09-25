#include "Application.h"

#include "../Scene/SceneFactory.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Application::Application()
    : m_window(1000, 700, "Task 3 - Polyhedra")
    , m_scene(SceneFactory::Create())
    , m_camera({ 0.0f, 0.0f, 8.0f }, { 0.0f, 0.0f, 0.0f }, 46.0f)
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
    m_renderer.Render(m_scene, m_camera, m_window.GetFramebufferSize());
}
