#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <cmath>

static constexpr float PI = 3.14159265358979323846f;

void Renderer::color(const Color& c) const 
{
    glColor4f(c.r, c.g, c.b, c.a);
}

void Renderer::quad(float x1, float y1, float x2, float y2, const Color& c) const 
{
    color(c);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void Renderer::gradientQuad(float x1, float y1, float x2, float y2,
    const Color& c1, const Color& c2,
    const Color& c3, const Color& c4) const 
{
    glBegin(GL_QUADS);
        color(c1); 
        glVertex2f(x1, y1);

        color(c2); 
        glVertex2f(x2, y1);

        color(c3);
        glVertex2f(x2, y2);

        color(c4); 
        glVertex2f(x1, y2);
    glEnd();
}

void Renderer::polygon(const std::vector<Vec2>& points, const Color& c) const 
{
    color(c);
    glBegin(GL_POLYGON);
        for (const Vec2& p : points) 
        {
            glVertex2f(p.x, p.y);
        }
    glEnd();
}

void Renderer::ellipse(const Vec2& center, float rx, float ry, const Color& c, int segments) const 
{
    color(c);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(center.x, center.y);
        for (int i = 0; i <= segments; ++i) 
        {
            float t = 2.0f * PI * static_cast<float>(i) / static_cast<float>(segments);
            glVertex2f(center.x + std::cos(t) * rx, center.y + std::sin(t) * ry);
        }
    glEnd();
}

void Renderer::ellipseOutline(const Vec2& center, float rx, float ry, const Color& c,
    float width, int segments) const 
{
    glLineWidth(width);
    color(c);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i) 
        {
            float t = 2.0f * PI * static_cast<float>(i) / static_cast<float>(segments);
            glVertex2f(center.x + std::cos(t) * rx, center.y + std::sin(t) * ry);
        }
    glEnd();
}

void Renderer::circle(const Vec2& center, float r, const Color& c, int segments) const 
{
    ellipse(center, r, r, c, segments);
}