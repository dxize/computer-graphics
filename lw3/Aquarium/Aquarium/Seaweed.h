#pragma once
#include "MathTypes.h"

class Renderer;

class Seaweed 
{
public:
    Seaweed(float x, float baseY, float height, float width, Color color, float phase);

    void draw(const Renderer& renderer, float time) const;

private:
    void drawBlade(float time, float xOffset, float scale, const Color& bladeColor) const;

    float x = 0.0f;
    float baseY = -34.0f;
    float height = 18.0f;
    float width = 1.0f;
    Color color{};
    float phase = 0.0f;
};