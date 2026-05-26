#include "Application.h"
#include "Constants.h"

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <cstdlib>
#include <iostream>

Application::Application()
    : m_surface(Constants::GRID_SIZE, Constants::GRID_SIZE),
    m_shader(Constants::VERTEX_SHADER_PATH, Constants::FRAGMENT_SHADER_PATH),
    m_width(Constants::WINDOW_WIDTH),
    m_height(Constants::WINDOW_HEIGHT),
    m_distance(Constants::CAMERA_DISTANCE_START)
{
}

Application::~Application()
{
    shutdown();
}

int Application::run()
{
    if (!init())
    {
        return EXIT_FAILURE;
    }

    loop();
    return EXIT_SUCCESS;
}

bool Application::init()
{
    if (!initGlfw()) return false;
    if (!createWindow()) return false;
    setupCallbacks();
    if (!initGlew()) return false;

    setupOpenGL();
    setupViewport();

    return buildScene();
}

bool Application::initGlfw()
{
    if (!glfwInit())
    {
        std::cout << "GLFW init error\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

bool Application::createWindow()
{
    m_window = glfwCreateWindow(
        Constants::WINDOW_WIDTH,
        Constants::WINDOW_HEIGHT,
        Constants::WINDOW_TITLE,
        nullptr,
        nullptr
    );

    if (!m_window)
    {
        std::cout << "Window creation error\n";
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    return true;
}

void Application::setupCallbacks()
{
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, onResize);
    glfwSetMouseButtonCallback(m_window, onMouseButton);
    glfwSetCursorPosCallback(m_window, onMouseMove);
    glfwSetScrollCallback(m_window, onMouseScroll);
}

bool Application::initGlew()
{
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW init error\n";
        return false;
    }

    return true;
}

void Application::setupOpenGL()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(
        Constants::BACKGROUND_R,
        Constants::BACKGROUND_G,
        Constants::BACKGROUND_B,
        Constants::BACKGROUND_A
    );
}

void Application::setupViewport()
{
    int width = 0;
    int height = 0;

    glfwGetFramebufferSize(m_window, &width, &height);
    resize(width, height);
}

bool Application::buildScene()
{
    m_surface.build();
    m_shader.build();

    return m_shader.isReady();
}

void Application::loop()
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

void Application::processInput()
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }

    bool wPressed = glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS;

    if (wPressed && !m_wasWPressed)
    {
        m_wireframe = !m_wireframe;
    }

    m_wasWPressed = wPressed;
}

void Application::updateShaderUniforms()
{
    m_shader.use();

    m_shader.setMat4("uModel", glm::mat4(1.0f));
    m_shader.setMat4("uView", viewMatrix());
    m_shader.setMat4("uProjection", projectionMatrix());

    m_shader.setFloat("uTime", static_cast<float>(glfwGetTime()));
    m_shader.setVec3("uLightPosition", glm::vec3(3.0f, -4.0f, 5.0f));
}

glm::mat4 Application::viewMatrix() const
{
    glm::vec3 eye(
        m_distance * std::cos(m_pitch) * std::cos(m_yaw),
        m_distance * std::cos(m_pitch) * std::sin(m_yaw),
        m_distance * std::sin(m_pitch)
    );

    glm::vec3 center(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 0.0f, 1.0f);

    return glm::lookAt(eye, center, up);
}

glm::mat4 Application::projectionMatrix() const
{
    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

    return glm::perspective(
        glm::radians(45.0f),
        aspect,
        0.1f,
        100.0f
    );
}

void Application::resize(int width, int height)
{
    m_width = width > 0 ? width : 1;
    m_height = height > 0 ? height : 1;

    glViewport(0, 0, m_width, m_height);
}

void Application::mouseButton(int button, int action)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
    {
        return;
    }

    m_mousePressed = action == GLFW_PRESS;
    glfwGetCursorPos(m_window, &m_lastMouseX, &m_lastMouseY);
}

void Application::mouseMove(double x, double y)
{
    if (!m_mousePressed)
    {
        return;
    }

    float dx = static_cast<float>(x - m_lastMouseX);
    float dy = static_cast<float>(y - m_lastMouseY);

    const float sensitivity = 0.006f;

    m_yaw += dx * sensitivity;
    m_pitch += dy * sensitivity;

    m_pitch = Math::clamp(
        m_pitch,
        glm::radians(-85.0f),
        glm::radians(85.0f)
    );

    m_lastMouseX = x;
    m_lastMouseY = y;
}

void Application::mouseScroll(double offset)
{
    const float sensitivity = 0.35f;

    m_distance -= static_cast<float>(offset) * sensitivity;

    m_distance = Math::clamp(
        m_distance,
        Constants::CAMERA_DISTANCE_MIN,
        Constants::CAMERA_DISTANCE_MAX
    );
}

void Application::shutdown()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

void Application::onResize(GLFWwindow* window, int width, int height)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->resize(width, height);
}

void Application::onMouseButton(GLFWwindow* window, int button, int action, int)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->mouseButton(button, action);
}

void Application::onMouseMove(GLFWwindow* window, double x, double y)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->mouseMove(x, y);
}

void Application::onMouseScroll(GLFWwindow* window, double, double yOffset)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->mouseScroll(yOffset);
}