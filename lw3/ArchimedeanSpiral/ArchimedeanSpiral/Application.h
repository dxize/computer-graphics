#pragma once

#include "Projection.h"
#include "Renderer.h"
#include "Spiral.h"

#include <GLFW/glfw3.h>

class Application
{
public:
    Application();
    ~Application();

    int run();

private:
    bool initialize();

    bool initializeGlfw();
    bool createWindow();
    void setWindowContext();
    void setOpenGL();
    void buildScene();
    void initializeProjection();

    void processInput();
    void mainLoop();
    void shutdown();

    void onFramebufferSizeChanged(int width, int height);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
    GLFWwindow* m_window;
    Spiral m_spiral;
    Projection m_projection;
    Renderer m_renderer;
};