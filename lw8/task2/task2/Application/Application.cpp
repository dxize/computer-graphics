#include "Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

Application::Application()
    : m_window(800, 600, "Task 2 - Soft shadows")
    , m_shader("Shaders/raytrace.vert", "Shaders/raytrace.frag")
{
    m_light = {
        { 1.3f, 2.0f, 1.5f },
        { 0.18f, 0.18f, 0.18f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        0.35f
    };

    m_material = {
        { 0.25f, 0.18f, 0.12f },
        { 0.9f, 0.55f, 0.3f },
        { 1.0f, 1.0f, 1.0f },
        64.0f
    };
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
    {
        m_window.Close();
    }
}

void Application::RenderFrame()
{
    const auto [width, height] = m_window.GetFramebufferSize();
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader.Use();
    m_shader.SetFloat("aspectRatio", static_cast<float>(width) / static_cast<float>(height));
    m_shader.SetVector("cameraPosition", m_cameraPosition);
    SetLightUniforms();
    SetMaterialUniforms();
    m_quad.Draw();
}

void Application::SetLightUniforms() const
{
    m_shader.SetVector("light.position", m_light.position);
    m_shader.SetColor("light.ambient", m_light.ambient);
    m_shader.SetColor("light.diffuse", m_light.diffuse);
    m_shader.SetColor("light.specular", m_light.specular);
    m_shader.SetFloat("light.radius", m_light.radius);
}

void Application::SetMaterialUniforms() const
{
    m_shader.SetColor("material.ambient", m_material.ambient);
    m_shader.SetColor("material.diffuse", m_material.diffuse);
    m_shader.SetColor("material.specular", m_material.specular);
    m_shader.SetFloat("material.shininess", m_material.shininess);
}
