#pragma once

#include "../Geometry/Cube.h"
#include "../Math/Vector3.h"
#include "../Rendering/RayTracingRenderer.h"
#include "../Window/Window.h"

class Application final
{
public:
    Application();
    void Run();

private:
    void HandleInput();
    void RenderFrame();

    Window m_window;
    RayTracingRenderer m_renderer;
    Cube m_shape;
    Vector3 m_cameraPosition{ 0.0f, 0.0f, 5.0f };
};
