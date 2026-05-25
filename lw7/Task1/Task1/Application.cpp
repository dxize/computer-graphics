#include "Application.h"

#include "Constants.h"

#include <cstdlib>
#include <iostream>

Application::Application()
    : m_window(nullptr),
    m_line(Constants::MIN_X, Constants::MAX_X, Constants::SEGMENTS),
    m_shader("Sinc.vert", "Color.frag")
{
}

Application::~Application()
{
    shutdown();
}

int Application::run()
{
    if (!initialize())
    {
        return 1;
    }

    mainLoop();
    return 0;
}

bool Application::initialize()
{
    if (!initializeGlfw())
    {
        return false;
    }

    if (!createWindow())
    {
        glfwTerminate();
        return false;
    }

    setWindowContext();

    if (!initializeGlew())
    {
        return false;
    }

    setOpenGL();
    buildScene();
    initializeProjection();

    return m_shader.isReady();
}

bool Application::initializeGlfw()
{
    if (!glfwInit())
    {
        std::cerr << "Не удалось инициализировать GLFW.\n";
        return false;
    }

    return true;
}

bool Application::createWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(
        Constants::INITIAL_WINDOW_WIDTH,
        Constants::INITIAL_WINDOW_HEIGHT,
        "Sinc function",
        nullptr,
        nullptr
    );

    return m_window != nullptr;
}

void Application::setWindowContext()
{
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
}

bool Application::initializeGlew()
{
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Не удалось инициализировать GLEW.\n";
        return false;
    }

    return true;
}

void Application::setOpenGL()
{
    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
    glLineWidth(2.0f);
}

void Application::buildScene()
{
    m_line.build();
    m_shader.build();
}

void Application::initializeProjection()
{
    int width = 0;
    int height = 0;

    glfwGetFramebufferSize(m_window, &width, &height);
    m_projection.update(width, height);
}

void Application::processInput()
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}

void Application::mainLoop()
{
    while (!glfwWindowShouldClose(m_window))
    {
        processInput();

        m_renderer.render(m_shader, m_line);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Application::shutdown()
{
    if (m_window != nullptr)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

void Application::onFramebufferSizeChanged(int width, int height)
{
    m_projection.update(width, height);
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app != nullptr)
    {
        app->onFramebufferSizeChanged(width, height);
    }
}