#pragma once

#include "Projection.h"
#include "Vec2.h"

#include <vector>

class Renderer
{
public:
    void render(const Projection& projection, const std::vector<Vec2>& spiralPoints) const;

private:
    void drawAxes(const Projection& projection) const;
    void drawSpiral(const std::vector<Vec2>& spiralPoints) const;

    void setupAxesStyle() const;
    void drawMainAxes(const Projection& projection, float arrowLen, float arrowHalfWidth) const;
    void drawTicks(float start, float end, float tickStep, float tickHalfLen, bool isXAxis) const;
};