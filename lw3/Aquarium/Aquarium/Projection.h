#pragma once

class Projection
{
public:
    void apply(int framebufferWidth, int framebufferHeight) const;

private:
    const float worldWidth = 100.0f;
    const float worldHeight = 75.0f;

    void calculateHalfSizes(int framebufferWidth, int framebufferHeight,
        float& halfW, float& halfH) const;
};