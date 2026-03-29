#include "Seaweed.h"
#include <GLFW/glfw3.h>
#include <cmath>

Seaweed::Seaweed(float x, float baseY, float height, float width, Color color, float phase)
    : x(x), baseY(baseY), height(height), width(width), color(color), phase(phase) 
{
}

void Seaweed::draw(const Renderer&, float time) const
{
    drawBlade(time, 0.0f, 1.0f, color);
    drawBlade(time, -0.9f, 0.75f, { color.r * 0.8f, color.g * 0.9f, color.b * 0.8f, 1.0f });
    drawBlade(time, 0.85f, 0.68f, { color.r * 0.7f, color.g * 0.85f, color.b * 0.7f, 1.0f });
}

void Seaweed::drawBlade(float time, float xOffset, float scale, const Color& bladeColor) const 
{
    glColor4f(bladeColor.r, bladeColor.g, bladeColor.b, bladeColor.a);

    glBegin(GL_TRIANGLE_STRIP);
        const int segments = 18;

        for (int i = 0; i <= segments; ++i) 
        {
            float t = static_cast<float>(i) / static_cast<float>(segments);
            float y = baseY + t * height * scale;
            float sway = std::sin(time * 1.7f + phase + t * 4.0f) * (1.0f + t * 1.7f);
            float cx = x + xOffset + sway;
            float half = width * scale * (1.0f - t * 0.55f);

            glVertex2f(cx - half, y);
            glVertex2f(cx + half, y);
        }

    glEnd();
}