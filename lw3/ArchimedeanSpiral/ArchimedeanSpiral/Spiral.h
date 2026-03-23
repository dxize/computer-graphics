#pragma once

#include "Vec2.h"

#include <vector>

class Spiral 
{
public:
    Spiral(float k, float phiMax, float phiStep);

    void build();

    const std::vector<Vec2>& getPoints() const;
    float getMaxRadius() const;

private:
    float m_k;
    float m_phiMax;
    float m_phiStep;
    std::vector<Vec2> m_points;
};
