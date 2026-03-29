#include "App.h"
#include <iostream>

bool App::init()
{
    if (!initGlfw())
    {
        return false;
    }

    if (!createMainWindow())
    {
        glfwTerminate();
        return false;
    }

    setupWindowCallbacks();
    setupOpenGLState();
    setupProjection();

    return true;
}

bool App::initGlfw()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed\n";
        return false;
    }
    return true;
}

bool App::createMainWindow()
{
    window = glfwCreateWindow(1280, 900, "Aquarium", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Window creation failed\n";
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    return true;
}

void App::setupWindowCallbacks()
{
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void App::setupOpenGLState()
{
    glEnable(GL_BLEND);//включает смешивание цветов.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);//включает сглаживание линий
}

void App::setupProjection()
{
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    projection.apply(framebufferWidth, framebufferHeight);
}

void App::run()
{
    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        float now = static_cast<float>(glfwGetTime());
        float dt = now - lastTime;
        lastTime = now;

        if (dt > 0.05f)
        {
            dt = 0.05f;
        }

        scene.update(dt, now);
        render(now);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

App::~App()
{
    if (window)
    {
        glfwDestroyWindow(window);
    }

    glfwTerminate();
}

void App::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
    if (!app)
    {
        return;
    }

    app->framebufferWidth = width;
    app->framebufferHeight = height;
    app->projection.apply(width, height);
}

void App::render(float time)
{
    glClearColor(0.08f, 0.20f, 0.35f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    scene.draw(renderer, time);
}