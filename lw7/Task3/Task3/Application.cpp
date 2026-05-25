#include "Application.h"

#include "Constants.h"

#include <cstdlib>
#include <iostream>

Application::Application()
    : m_window(nullptr),
    m_surface(Constants::SURFACE_SEGMENTS_U, Constants::SURFACE_SEGMENTS_V),
    m_shader(Constants::VERTEX_SHADER_PATH, Constants::FRAGMENT_SHADER_PATH),
    m_yaw(0.0f),
    m_pitch(0.25f),
    m_distance(Constants::CAMERA_DISTANCE_START),
    m_leftMousePressed(false),
    m_wireframe(false),
    m_wasWPressed(false),
    m_lastMouseX(0.0),
    m_lastMouseY(0.0)
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
        Constants::WINDOW_TITLE,
        nullptr,
        nullptr
    );

    if (m_window == nullptr)
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
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetCursorPosCallback(m_window, cursorPositionCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
}

bool Application::initializeGlew()
{
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Не удалось инициализировать GLEW.\n";
        return false;
    }

    glGetError();
    return true;
}

void Application::setOpenGL()
{
    glClearColor(
        Constants::BACKGROUND_R,
        Constants::BACKGROUND_G,
        Constants::BACKGROUND_B,
        Constants::BACKGROUND_A
    );

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void Application::buildScene()
{
    m_surface.build();
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

    const bool wPressed = glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS;

    if (wPressed && !m_wasWPressed)
    {
        m_wireframe = !m_wireframe;
    }

    m_wasWPressed = wPressed;
}

void Application::updateShaderUniforms()
{
    m_shader.use();

    m_shader.setMat4("uModel", Mat4::identity());
    m_shader.setMat4("uView", viewMatrix());
    m_shader.setMat4("uProjection", projectionMatrix());

    m_shader.setFloat("uTime", static_cast<float>(glfwGetTime()));

    m_shader.setVec3("uLightPosition", { 2.3f, 3.0f, 2.4f });
    m_shader.setVec3("uBaseColor", { 0.08f, 0.28f, 1.0f });
}

void Application::mainLoop()
{
    while (!glfwWindowShouldClose(m_window))
    {
        processInput();
        updateShaderUniforms();

        m_renderer.render(m_shader, m_surface, m_wireframe);

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

Vec3 Application::cameraPosition() const
{
    return
    {
        m_distance * std::cos(m_pitch) * std::sin(m_yaw),
        m_distance * std::sin(m_pitch),
        m_distance * std::cos(m_pitch) * std::cos(m_yaw)
    };
}

Mat4 Application::viewMatrix() const
{
    return Mat4::lookAt(
        cameraPosition(),
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f }
    );
}

Mat4 Application::projectionMatrix() const
{
    return Mat4::perspective(
        Math::radians(45.0f),
        m_projection.aspectRatio(),
        0.1f,
        100.0f
    );
}

void Application::onFramebufferSizeChanged(int width, int height)
{
    m_projection.update(width, height);
}

void Application::onMouseButtonChanged(int button, int action)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
    {
        return;
    }

    if (action == GLFW_PRESS)
    {
        m_leftMousePressed = true;
        glfwGetCursorPos(m_window, &m_lastMouseX, &m_lastMouseY);
    }
    else if (action == GLFW_RELEASE)
    {
        m_leftMousePressed = false;
    }
}

void Application::onCursorMoved(double x, double y)
{
    if (!m_leftMousePressed)
    {
        return;
    }

    const double dx = x - m_lastMouseX;
    const double dy = y - m_lastMouseY;

    m_lastMouseX = x;
    m_lastMouseY = y;

    m_yaw += static_cast<float>(dx) * 0.006f;
    m_pitch += static_cast<float>(dy) * 0.006f;

    m_pitch = Math::clamp(
        m_pitch,
        Math::radians(-85.0f),
        Math::radians(85.0f)
    );
}

void Application::onMouseScrolled(double yOffset)
{
    m_distance -= static_cast<float>(yOffset) * 0.25f;

    m_distance = Math::clamp(
        m_distance,
        Constants::CAMERA_DISTANCE_MIN,
        Constants::CAMERA_DISTANCE_MAX
    );
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app != nullptr)
    {
        app->onFramebufferSizeChanged(width, height);
    }
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    (void)mods;

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app != nullptr)
    {
        app->onMouseButtonChanged(button, action);
    }
}

void Application::cursorPositionCallback(GLFWwindow* window, double x, double y)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app != nullptr)
    {
        app->onCursorMoved(x, y);
    }
}

void Application::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    (void)xOffset;

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app != nullptr)
    {
        app->onMouseScrolled(yOffset);
    }
}
