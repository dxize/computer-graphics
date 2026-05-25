#pragma once

class Projection
{
public:
    Projection();

    void update(int width, int height);
    float aspectRatio() const;

private:
    void normalizeSize(int width, int height);
    void applyViewport() const;

private:
    int m_width;
    int m_height;
};
