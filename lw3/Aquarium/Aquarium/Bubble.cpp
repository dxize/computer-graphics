#include "Bubble.h"
#include "Renderer.h"
#include <cmath>

Bubble::Bubble(Vec2 start, float radius, float speed, float drift, float phase)
    : pos(start), radius(radius), speed(speed), drift(drift), phase(phase) 
{
}

void Bubble::update(float dt, float time) 
{
    pos.y += speed * dt;
    pos.x += std::sin(time * 2.2f + phase) * drift * dt;
    alive = pos.y < 37.0f;
}

void Bubble::draw(const Renderer& renderer) const 
{
    renderer.circle(pos, radius, { 0.82f, 0.95f, 1.0f, 0.22f }, 24);
    renderer.ellipseOutline(pos, radius, radius, { 0.88f, 0.98f, 1.0f, 0.75f }, 1.5f, 24);
    renderer.circle({ pos.x - radius * 0.3f, pos.y + radius * 0.25f },
        radius * 0.22f, { 1.0f, 1.0f, 1.0f, 0.65f }, 16);
}

bool Bubble::isAlive() const
{
    return alive;
}