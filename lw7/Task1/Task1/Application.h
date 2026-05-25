#pragma once

#include "Projection.h"
#include "Renderer.h"
#include "SincLine.h"
#include "ShaderProgram.h"

#include <GL/glew.h>
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
    bool initializeGlew();

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

    Projection m_projection;
    Renderer m_renderer;
    SincLine m_line;
    ShaderProgram m_shader;
};