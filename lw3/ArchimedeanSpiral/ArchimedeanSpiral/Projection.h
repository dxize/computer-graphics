#pragma once

class Projection
{
public:
    Projection();

    void configure(float maxRadius, int minTicksPerAxis);
    void update(int framebufferWidth, int framebufferHeight);

    float getLeft() const;
    float getRight() const;
    float getBottom() const;
    float getTop() const;
    float getTickStep() const;

private:
    void normalizeFramebufferSize(int framebufferWidth, int framebufferHeight);
    void calculateProjectionBounds();
    void applyProjection() const;

private:
    int m_framebufferWidth;
    int m_framebufferHeight;

    float m_left;
    float m_right;
    float m_bottom;
    float m_top;

    float m_baseHalfExtent;
    float m_tickStep;
};