#include "Application.h"

#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>

static const float PI = 3.1415f;

void Application::Run()
{
    if (!Initialize())
    {
        Shutdown();
        return;
    }

    float previousTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(m_window))
    {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        Update(deltaTime);
        Render();

        glfwPollEvents();
    }

    Shutdown();
}

bool Application::Initialize()
{
    if (!InitGLFW())
    {
        return false;
    }

    SetupWindowHints();

    if (!CreateWindow())
    {
        return false;
    }

    SetupCallbacks();
    SetupOpenGL();

    if (!LoadScene())
    {
        return false;
    }

    return true;
}

bool Application::InitGLFW()
{
    if (!glfwInit())
    {
        std::cout << "Не удалось инициализировать GLFW\n";
        return false;
    }

    return true;
}

void Application::SetupWindowHints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
}

bool Application::CreateWindow()
{
    m_window = glfwCreateWindow(
        m_width,
        m_height,
        "Chess Scene",
        nullptr,
        nullptr
    );

    if (m_window == nullptr)
    {
        std::cout << "Не удалось создать окно GLFW\n";
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    return true;
}

void Application::SetupCallbacks()
{
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, HandleFramebufferResize);
}

void Application::SetupOpenGL()
{
    ConfigureOpenGl();
    ConfigureProjection(m_width, m_height);
}

bool Application::LoadScene()
{
    if (!m_scene.Load())
    {
        std::cout << "Не удалось загрузить сцену\n";
        return false;
    }

    return true;
}

void Application::Shutdown()
{
    if (m_window != nullptr)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}
    
void Application::ConfigureOpenGl()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);

    glClearColor(0.08f, 0.09f, 0.14f, 1.0f);

    ConfigureLighting();
}

void Application::ConfigureProjection(int width, int height)
{
    if (height <= 0)
    {
        height = 1;
    }

    m_width = width;
    m_height = height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = static_cast<float>(width) / static_cast<float>(height);

    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float fovDegrees = 45.0f;

    float top = std::tan(fovDegrees * PI / 360.0f) * nearPlane;
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    glFrustum(left, right, bottom, top, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
}

void Application::ConfigureLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    float lightPosition[] =
    {
        0.0f, 5.0f, 2.5f, 1.0f
    };

    float lightAmbient[] =
    {
        0.4f, 0.4f, 0.4f, 1.0f
    };

    float lightDiffuse[] =
    {
        0.75f, 0.75f, 0.70f, 1.0f
    };

    float lightSpecular[] =
    {
        0.20f, 0.20f, 0.20f, 1.0f
    };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.02f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.002f);
}

void Application::Update(float deltaTime)
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }

    bool isRestartPressed = glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS;

    if (isRestartPressed && !m_wasRestartPressed)
    {
        m_scene.Restart();
    }

    m_wasRestartPressed = isRestartPressed;

    m_camera.Update(m_window, deltaTime);
    m_scene.Update(deltaTime);
}

void Application::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    m_camera.Apply();

    ConfigureLighting();

    m_scene.Draw();

    glfwSwapBuffers(m_window);
}

void Application::HandleFramebufferResize(GLFWwindow* window, int width, int height)
{
    Application* application =
        static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (application != nullptr)
    {
        application->ConfigureProjection(width, height);
    }
}