#pragma once
#include <vector>
#include "MathTypes.h"

class Renderer 
{
public:
    void color(const Color& c) const;

    void gradientQuad(float x1, float y1, float x2, float y2,
        const Color& c1, const Color& c2,
        const Color& c3, const Color& c4) const;

    void polygon(const std::vector<Vec2>& points, const Color& c) const;

    void ellipse(const Vec2& center, float rx, float ry, const Color& c, int segments = 48) const;

    void ellipseOutline(const Vec2& center, float rx, float ry, const Color& c,
        float width = 1.0f, int segments = 48) const;

    void circle(const Vec2& center, float r, const Color& c, int segments = 32) const;
};