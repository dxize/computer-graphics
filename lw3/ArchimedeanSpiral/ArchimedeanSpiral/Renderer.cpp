#include "Renderer.h"

#include <GLFW/glfw3.h>

#include <cmath>

void Renderer::render(const Projection& projection, const std::vector<Vec2>& spiralPoints) const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawAxes(projection);
    drawSpiral(spiralPoints);
}

void Renderer::drawAxes(const Projection& projection) const
{
    const float tickStep = projection.getTickStep();
    const float tickHalfLen = tickStep * 0.18f;
    const float arrowLen = tickStep * 0.65f;
    const float arrowHalfWidth = tickStep * 0.28f;

    setupAxesStyle();
    drawMainAxes(projection, arrowLen, arrowHalfWidth);
    drawTicks(projection.getLeft(), projection.getRight(), tickStep, tickHalfLen, true);
    drawTicks(projection.getBottom(), projection.getTop(), tickStep, tickHalfLen, false);
}

void Renderer::drawSpiral(const std::vector<Vec2>& spiralPoints) const
{
    glColor3f(0.85f, 0.10f, 0.10f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_STRIP);
    for (const Vec2& point : spiralPoints)
    {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

void Renderer::setupAxesStyle() const
{
    glColor3f(0.15f, 0.15f, 0.15f);
    glLineWidth(2.0f);
}

void Renderer::drawMainAxes(const Projection& projection, float arrowLen, float arrowHalfWidth) const
{
    const float left = projection.getLeft();
    const float right = projection.getRight();
    const float bottom = projection.getBottom();
    const float top = projection.getTop();

    glBegin(GL_LINES);

    glVertex2f(left, 0.0f);
    glVertex2f(right, 0.0f);

    glVertex2f(0.0f, bottom);
    glVertex2f(0.0f, top);

    glVertex2f(right, 0.0f);
    glVertex2f(right - arrowLen, arrowHalfWidth);

    glVertex2f(right, 0.0f);
    glVertex2f(right - arrowLen, -arrowHalfWidth);

    glVertex2f(0.0f, top);
    glVertex2f(arrowHalfWidth, top - arrowLen);

    glVertex2f(0.0f, top);
    glVertex2f(-arrowHalfWidth, top - arrowLen);

    glEnd();
}

void Renderer::drawTicks(float start, float end, float tickStep, float tickHalfLen, bool isXAxis) const
{
    glBegin(GL_LINES);

    for (float value = std::ceil(start / tickStep) * tickStep; value <= end; value += tickStep)
    {
        if (std::fabs(value) < 1e-5f)
        {
            continue;
        }

        if (isXAxis)
        {
            glVertex2f(value, -tickHalfLen);
            glVertex2f(value, tickHalfLen);
        }
        else
        {
            glVertex2f(-tickHalfLen, value);
            glVertex2f(tickHalfLen, value);
        }
    }

    glEnd();
}