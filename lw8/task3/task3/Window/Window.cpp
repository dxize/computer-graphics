#include "Window.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>

Window::Window(int width, int height, const char* title)
{
    Create(width, height, title);
    InitializeOpenGL();
    InitializeResizeTracking();
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

void Window::Create(int width, int height, const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window)
        throw std::runtime_error("Failed to create GLFW window");
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
}

void Window::InitializeOpenGL()
{
    const auto loader = reinterpret_cast<GLADloadproc>(glfwGetProcAddress);
    if (!gladLoadGLLoader(loader))
        throw std::runtime_error("Failed to initialize GLAD");
}

void Window::InitializeResizeTracking()
{
    glfwSetWindowUserPointer(m_window, this);
    glfwGetFramebufferSize(m_window, &m_framebufferSize.width, &m_framebufferSize.height);
    glfwSetFramebufferSizeCallback(m_window, &Window::OnFramebufferSize);
}

void Window::OnFramebufferSize(GLFWwindow* window, int width, int height)
{
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (self)
        self->m_framebufferSize = { width, height };
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_window) == GLFW_TRUE;
}

void Window::Close()
{
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(m_window);
}

FramebufferSize Window::GetFramebufferSize() const
{
    return m_framebufferSize;
}

GLFWwindow* Window::GetHandle() const
{
    return m_window;
}
