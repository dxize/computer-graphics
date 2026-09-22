#include "Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

Application::Application()
    : m_window(800, 600, "Phong Lighting")
    , m_shader("Shaders/raytrace.vert", "Shaders/raytrace.frag")
{
    m_light = {
        { 2.0f, 3.0f, 5.0f },
        { 0.2f, 0.2f, 0.2f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f }
    };

    m_material = {
        { 0.3f, 0.2f, 0.15f },
        { 1.0f, 0.7f, 0.5f },
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
}

void Application::SetMaterialUniforms() const
{
    m_shader.SetColor("material.ambient", m_material.ambient);
    m_shader.SetColor("material.diffuse", m_material.diffuse);
    m_shader.SetColor("material.specular", m_material.specular);
    m_shader.SetFloat("material.shininess", m_material.shininess);
}
