#pragma once

#include <GLFW/glfw3.h>
#include "Projection.h"
#include "Renderer.h"
#include "AquariumScene.h"

class App
{
public:
    bool init();
    void run();
    ~App();

private:
    bool initGlfw();
    bool createMainWindow();
    void setupWindowCallbacks();
    void setupOpenGLState();
    void setupProjection();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void render(float time);

private:
    GLFWwindow* window = nullptr;
    int framebufferWidth = 1280;
    int framebufferHeight = 900;

    Projection projection;
    Renderer renderer;
    AquariumScene scene;
};