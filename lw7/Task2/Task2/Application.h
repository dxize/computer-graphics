#pragma once

#include "FlagRectangle.h"
#include "Projection.h"
#include "Renderer.h"
#include "ShaderProgram.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Application
{
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

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
    FlagRectangle m_flag;
    ShaderProgram m_shader;
};
