#include "Application.h"

#include "Constants.h"

#include <cstdlib>
#include <iostream>

Application::Application()
    : m_window(nullptr),
    m_spiral(Constants::K, Constants::PHI_MAX, Constants::PHI_STEP)
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
        return EXIT_FAILURE;
    }

    mainLoop();
    return EXIT_SUCCESS;
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
    setOpenGL();
    buildScene();
    initializeProjection();

    return true;
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
    m_window = glfwCreateWindow(
        Constants::INITIAL_WINDOW_WIDTH,
        Constants::INITIAL_WINDOW_HEIGHT,
        "Архимедова спираль",
        nullptr, //оконный формат
        nullptr  //ссылка на другое окно, с которым можно делить OpenGL-ресурсы(списки вершин, буферы)
    );

    if (!m_window)
    {
        std::cerr << "Не удалось создать окно GLFW.\n";
        return false;
    }

    return true;
}

void Application::setWindowContext()
{
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
}

void Application::setOpenGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_LINE_SMOOTH);//сглаживание
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);//смешивание цветов
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::buildScene()
{
    m_spiral.build();
    m_projection.configure(m_spiral.getMaxRadius(), Constants::MIN_TICKS_PER_AXIS);
}

void Application::initializeProjection()
{
    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);
    m_projection.update(framebufferWidth, framebufferHeight);
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
        m_renderer.render(m_projection, m_spiral.getPoints());
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