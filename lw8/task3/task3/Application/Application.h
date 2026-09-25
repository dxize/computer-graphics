#pragma once

#include "../Rendering/Camera.h"
#include "../Rendering/RayTracingRenderer.h"
#include "../Scene/Scene.h"
#include "../Window/GlfwContext.h"
#include "../Window/Window.h"

class Application final
{
public:
    Application();
    void Run();

private:
    void HandleInput();
    void RenderFrame();

    GlfwContext m_glfw;
    Window m_window;
    RayTracingRenderer m_renderer;
    Scene m_scene;
    Camera m_camera;
};
