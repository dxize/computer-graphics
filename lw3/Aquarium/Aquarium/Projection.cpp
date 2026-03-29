#include "Projection.h"
#include <GLFW/glfw3.h>

void Projection::calculateHalfSizes(int framebufferWidth, int framebufferHeight,
    float& halfW, float& halfH) const
{
    const float windowAspect =
        static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight);

    const float worldAspect = worldWidth / worldHeight;

    halfW = worldWidth * 0.5f;
    halfH = worldHeight * 0.5f;

    if (windowAspect > worldAspect)
    {
        halfW *= windowAspect / worldAspect;
    }
    else
    {
        halfH *= worldAspect / windowAspect;
    }
}

void Projection::apply(int framebufferWidth, int framebufferHeight) const
{
    if (framebufferWidth <= 0 || framebufferHeight <= 0)
    {
        return;
    }

    glViewport(0, 0, framebufferWidth, framebufferHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float halfW = 0.0f;
    float halfH = 0.0f;
    calculateHalfSizes(framebufferWidth, framebufferHeight, halfW, halfH);

    glOrtho(-halfW, halfW, -halfH, halfH, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}