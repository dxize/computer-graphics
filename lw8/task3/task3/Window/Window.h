#pragma once

#include "FramebufferSize.h"

struct GLFWwindow;

class Window final
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool ShouldClose() const;
    void Close();
    void SwapBuffers() const;
    FramebufferSize GetFramebufferSize() const;
    GLFWwindow* GetHandle() const;

private:
    void Create(int width, int height, const char* title);
    void InitializeOpenGL();
    void InitializeResizeTracking();
    static void OnFramebufferSize(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window = nullptr;
    FramebufferSize m_framebufferSize{};
};
