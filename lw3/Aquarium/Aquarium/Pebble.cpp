#include "Pebble.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>

Pebble::Pebble(Vec2 pos, float scale, Kind kind)
    : pos(pos), scale(scale), kind(kind) 
{
}

void Pebble::draw(const Renderer& renderer) const 
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0.0f);
    glScalef(scale, scale, 1.0f);

    switch (kind) 
    {
    case Kind::Round:
        renderer.ellipse({ 0.0f, 0.0f }, 2.0f, 1.3f, { 0.58f, 0.55f, 0.52f, 1.0f });
        renderer.ellipse({ -0.4f, 0.1f }, 0.5f, 0.35f, { 0.70f, 0.68f, 0.66f, 0.7f });
        break;
    case Kind::Flat:
        renderer.ellipse({ 0.0f, 0.0f }, 2.7f, 0.9f, { 0.45f, 0.40f, 0.36f, 1.0f });
        renderer.ellipse({ 0.8f, 0.1f }, 0.8f, 0.22f, { 0.62f, 0.58f, 0.55f, 0.6f });
        break;
    case Kind::Tall:
        renderer.ellipse({ 0.0f, 0.0f }, 1.4f, 1.9f, { 0.50f, 0.47f, 0.44f, 1.0f });
        break;
    case Kind::Triangle:
        renderer.polygon({ {-2.0f, -1.0f}, {0.0f, 1.5f}, {2.0f, -1.0f} }, { 0.42f, 0.38f, 0.34f, 1.0f });
        break;  
    case Kind::Dark:
        renderer.ellipse({ 0.0f, 0.0f }, 2.2f, 1.5f, { 0.32f, 0.33f, 0.36f, 1.0f });
        break;
    case Kind::Light:
        renderer.ellipse({ 0.0f, 0.0f }, 2.0f, 1.2f, { 0.72f, 0.69f, 0.64f, 1.0f });
        break;
    }

    glPopMatrix();
}