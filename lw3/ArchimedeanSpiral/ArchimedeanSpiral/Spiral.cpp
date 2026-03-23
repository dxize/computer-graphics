#include "Spiral.h"

#include <cmath>

Spiral::Spiral(float k, float phiMax, float phiStep)
    : m_k(k), m_phiMax(phiMax), m_phiStep(phiStep)
{
}

void Spiral::build() 
{
    m_points.clear();
    m_points.reserve(static_cast<std::size_t>(m_phiMax / m_phiStep) + 2);

    for (float phi = 0.0f; phi < m_phiMax; phi += m_phiStep)
    {
        const float r = m_k * phi;
        m_points.push_back({ r * std::cos(phi), r * std::sin(phi) });
    }

    const float r = m_k * m_phiMax;
    m_points.push_back({ r * std::cos(m_phiMax), r * std::sin(m_phiMax) });
}

const std::vector<Vec2>& Spiral::getPoints() const
{
    return m_points;
}

float Spiral::getMaxRadius() const 
{
    return m_k * m_phiMax;
}
