#pragma once

#include "Math.h"
#include "MorphSurface.h"
#include "Renderer.h"
#include "ShaderProgram.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>

class Application
{
public:
    Application();
    ~Application();

    int run();

private:
    bool init();
    bool initGlfw();
    bool createWindow();
    bool initGlew();
    bool buildScene();

    void setupCallbacks();
    void setupOpenGL();
    void setupViewport();

    void loop();
    void processInput();
    void updateShaderUniforms();
    void shutdown();

    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;

    void resize(int width, int height);
    void mouseButton(int button, int action);
    void mouseMove(double x, double y);
    void mouseScroll(double offset);

    static void onResize(GLFWwindow* window, int width, int height);
    static void onMouseButton(GLFWwindow* window, int button, int action, int);
    static void onMouseMove(GLFWwindow* window, double x, double y);
    static void onMouseScroll(GLFWwindow* window, double, double yOffset);

private:
    GLFWwindow* m_window = nullptr;

    Renderer m_renderer;
    MorphSurface m_surface;
    ShaderProgram m_shader;

    int m_width = 1000;
    int m_height = 700;

    float m_yaw = 0.0f;
    float m_pitch = 0.45f;
    float m_distance = 6.0f;

    bool m_mousePressed = false;
    bool m_wireframe = false;
    bool m_wasWPressed = false;

    double m_lastMouseX = 0.0;
    double m_lastMouseY = 0.0;
};