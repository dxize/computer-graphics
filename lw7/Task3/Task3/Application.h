#pragma once

#include "Math.h"
#include "MorphSurface.h"
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
    void updateShaderUniforms();
    void mainLoop();
    void shutdown();

    Vec3 cameraPosition() const;
    Mat4 viewMatrix() const;
    Mat4 projectionMatrix() const;

    void onFramebufferSizeChanged(int width, int height);
    void onMouseButtonChanged(int button, int action);
    void onCursorMoved(double x, double y);
    void onMouseScrolled(double yOffset);

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double x, double y);
    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

private:
    GLFWwindow* m_window;

    Projection m_projection;
    Renderer m_renderer;
    MorphSurface m_surface;
    ShaderProgram m_shader;

    float m_yaw;
    float m_pitch;
    float m_distance;

    bool m_leftMousePressed;
    bool m_wireframe;
    bool m_wasWPressed;

    double m_lastMouseX;
    double m_lastMouseY;
};
