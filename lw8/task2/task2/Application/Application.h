#pragma once

#include "../Lighting/Light.h"
#include "../Materials/Material.h"
#include "../Math/Vector3.h"
#include "../Rendering/FullscreenQuad.h"
#include "../Rendering/ShaderProgram.h"
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
    void SetLightUniforms() const;
    void SetMaterialUniforms() const;

    GlfwContext m_glfw;
    Window m_window;
    ShaderProgram m_shader;
    FullscreenQuad m_quad;

    Vector3 m_cameraPosition{ 0.0f, 0.3f, 3.0f };
    Light m_light;
    Material m_material;
};
