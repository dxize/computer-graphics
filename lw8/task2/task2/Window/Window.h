#pragma once

#include <utility>

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
    std::pair<int, int> GetFramebufferSize() const;
    GLFWwindow* GetHandle() const;

private:
    void Create(int width, int height, const char* title);
    void InitializeOpenGL();

    GLFWwindow* m_window = nullptr;
};
