#include "Projection.h"

#include "MathUtils.h"
#include "Constants.h"

#include <GLFW/glfw3.h>

Projection::Projection()
    : m_framebufferWidth(Constants::INITIAL_WINDOW_WIDTH),
    m_framebufferHeight(Constants::INITIAL_WINDOW_HEIGHT),
    m_left(-10.0f),
    m_right(10.0f),
    m_bottom(-10.0f),
    m_top(10.0f),
    m_baseHalfExtent(1.0f),
    m_tickStep(1.0f)
{
}

void Projection::configure(float maxRadius, int minTicksPerAxis)
{
    const float maxAllowedStep = (2.0f * maxRadius) / static_cast<float>(minTicksPerAxis); 
    m_tickStep = MathUtils::niceStepFloor(maxAllowedStep);
    m_baseHalfExtent = maxRadius + 2.0f * m_tickStep;
}

void Projection::update(int framebufferWidth, int framebufferHeight)
{
    normalizeFramebufferSize(framebufferWidth, framebufferHeight);
    calculateProjectionBounds();
    applyProjection();
}

float Projection::getLeft() const   
{
    return m_left;
}

float Projection::getRight() const
{
    return m_right;
}

float Projection::getBottom() const
{
    return m_bottom;
}

float Projection::getTop() const
{
    return m_top;
}

float Projection::getTickStep() const
{
    return m_tickStep;
}

void Projection::normalizeFramebufferSize(int framebufferWidth, int framebufferHeight)
{
    m_framebufferWidth = framebufferWidth > 0 ? framebufferWidth : 1;
    m_framebufferHeight = framebufferHeight > 0 ? framebufferHeight : 1;
}

void Projection::calculateProjectionBounds()
{
    const float windowAspect = static_cast<float>(m_framebufferWidth) /
        static_cast<float>(m_framebufferHeight);

    float halfWidth = m_baseHalfExtent;
    float halfHeight = m_baseHalfExtent;

    if (windowAspect >= 1.0f)
    {
        halfWidth *= windowAspect;
    }
    else
    {
        halfHeight /= windowAspect;
    }

    m_left = -halfWidth;
    m_right = halfWidth;
    m_bottom = -halfHeight;
    m_top = halfHeight;
}

void Projection::applyProjection() const
{
    glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(m_left, m_right, m_bottom, m_top, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}