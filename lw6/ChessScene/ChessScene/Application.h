#pragma once

#include "ChessScene.h"
#include "OrbitCamera.h"

#include <GLFW/glfw3.h>

class Application
{
public:
    void Run();

private:
    bool Initialize();
    void Shutdown();

    bool InitGLFW();
    void SetupWindowHints();
    bool CreateWindow();
    void SetupCallbacks();
    void SetupOpenGL();
    bool LoadScene();
    void ConfigureOpenGl();

    void ConfigureProjection(int width, int height);
    void ConfigureLighting();

    void Update(float deltaTime);
    void Render();

    static void HandleFramebufferResize(GLFWwindow* window, int width, int height);

private:
    GLFWwindow* m_window = nullptr;

    ChessScene m_scene;
    OrbitCamera m_camera;

    int m_width = 1280;
    int m_height = 720;

    bool m_wasRestartPressed = false;
};