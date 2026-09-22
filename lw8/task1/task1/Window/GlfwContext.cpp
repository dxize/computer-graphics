#include "GlfwContext.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

GlfwContext::GlfwContext()
{
    if (glfwInit() == GLFW_FALSE)
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

GlfwContext::~GlfwContext()
{
    glfwTerminate();
}
