#include "Projection.h"

#include "Constants.h"

#include <GL/glew.h>

Projection::Projection()
    : m_width(Constants::INITIAL_WINDOW_WIDTH),
    m_height(Constants::INITIAL_WINDOW_HEIGHT)
{
}

void Projection::update(int width, int height)
{
    normalizeSize(width, height);
    applyViewport();
}

float Projection::aspectRatio() const
{
    return static_cast<float>(m_width) / static_cast<float>(m_height);
}

void Projection::normalizeSize(int width, int height)
{
    m_width = width > 0 ? width : 1;
    m_height = height > 0 ? height : 1;
}

void Projection::applyViewport() const
{
    glViewport(0, 0, m_width, m_height);
}
