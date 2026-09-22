#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

Window::Window(int width, int height, const char* title)
{
    Create(width, height, title);
    InitializeOpenGL();
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
    {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_window);
}

void Window::InitializeOpenGL()
{
    const auto loader = reinterpret_cast<GLADloadproc>(glfwGetProcAddress);
    if (!gladLoadGLLoader(loader))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
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

std::pair<int, int> Window::GetFramebufferSize() const
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    return { width, height };
}

GLFWwindow* Window::GetHandle() const
{
    return m_window;
}
