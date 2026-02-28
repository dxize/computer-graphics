#pragma once
#include <cmath>

class BallisticJump 
{
public:
    BallisticJump(float v0, float g) 
        : m_v0(v0), m_g(g) 
    {
        m_period = 2.f * m_v0 / m_g;
    }

    float yOffset(float t, float phase) const
    {
        float tau = std::fmod(t + phase, m_period);
        float h = m_v0 * tau - 0.5f * m_g * tau * tau;
        return (h > 0.f) ? h : 0.f;
    }       

private:
    float m_v0;
    float m_g;
    float m_period;
};