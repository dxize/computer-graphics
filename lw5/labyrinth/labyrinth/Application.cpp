#include "Application.h"

#include <GLFW/glfw3.h>

Application::Application()
    : window(nullptr), lastTime(0.0f), deltaTime(0.0f), width(1280), height(720)
{
}

int Application::run()
{
    if (!initGlfw())
        return 1;

    if (!createWindow())
    {
        shutdown();
        return 1;
    }

    setupWindow();
    initScene();
    runLoop();
    shutdown();
    return 0;
}

bool Application::initGlfw()
{
    return glfwInit() == GLFW_TRUE;
}

bool Application::createWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(width, height, "3D Labyrinth", nullptr, nullptr);
    return window != nullptr;
}

void Application::setupWindow()
{
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void Application::initScene()
{
    renderer.initOpenGL();
    renderer.setProjection(width, height);
    lastTime = static_cast<float>(glfwGetTime());
}

void Application::runLoop()
{
    while (!shouldClose())
    {
        updateTime();
        handleInput();
        renderFrame();
        presentFrame();
        pollEvents();
    }
}

void Application::updateTime()
{
    float currentTime = static_cast<float>(glfwGetTime());
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}

void Application::handleInput()
{
    inputHandler.handle(window, player, maze, deltaTime);
}

void Application::renderFrame() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer.drawScene(maze, player);
}

void Application::presentFrame() const
{
    glfwSwapBuffers(window);
}

void Application::pollEvents() const
{
    glfwPollEvents();
}

bool Application::shouldClose() const
{
    return glfwWindowShouldClose(window) == GLFW_TRUE;
}

void Application::shutdown()
{
    if (window != nullptr)
    {
        glfwMakeContextCurrent(window);
        renderer.shutdown();
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

void Application::onResize(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    renderer.setProjection(width, height);
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
        app->onResize(width, height);
}
